#include "system.hpp"
#include "filter/base.hpp"
#include "filter/unary.hpp"
#include "filter/binary.hpp"
#include "../../media_path.hpp"
#include "../screen_vf/format.hpp"
#include "../screen_vf/create_quad.hpp"
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/io/cerr.hpp>
#include <boost/foreach.hpp>
#include <boost/next_prior.hpp>
#include <boost/graph/topological_sort.hpp>
#include <iterator>
#include <iostream>
#include <utility>
#include <typeinfo>
#include <list>

fruitcut::sandbox::pp::system::system(
	sge::renderer::device_ptr const _renderer)
:
	renderer_(
		_renderer),
	screen_texture_(
		renderer_->create_texture(
			fcppt::math::dim::structure_cast<sge::renderer::dim2>(
				renderer_->screen_size()),
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	screen_target_(
		renderer_->create_target(
			screen_texture_,
			sge::renderer::no_depth_stencil_texture())),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"target_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					renderer_->screen_size()))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"tex"))),
	quad_(
		screen_vf::create_quad(
			shader_,
			renderer_))
{
}

void
fruitcut::sandbox::pp::system::update(
	scene_render_callback const &callback)
{
	render_scene(
		callback);
	apply_postprocessing();
}

void
fruitcut::sandbox::pp::system::render_result()
{
	shader_.update_texture(
		"tex",
		result_texture_);

	sge::shader::scoped scoped_shader(
		shader_);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		quad_);

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			quad_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);
}

void
fruitcut::sandbox::pp::system::add_filter(
	filter::base &b,
	fcppt::string const &name,
	dependency_set const &deps)
{
	vertex_descriptor const new_vertex = 
		boost::add_vertex(
			graph_);

	vertex_to_filter_[new_vertex] = 
		filter::wrapper(
			b,
			name);
	name_to_vertex_[name] = 
		new_vertex;

	BOOST_FOREACH(
		dependency_set::const_reference r,
		deps)
	{
		FCPPT_ASSERT_MESSAGE(
			name_to_vertex_.find(r) == name_to_vertex_.end(),
			FCPPT_TEXT("Filter ")+
			r+
			FCPPT_TEXT(" which was specified as the dependency for ")+
			name+
			FCPPT_TEXT(" was not found"));
			
		boost::add_edge(
			name_to_vertex_[r],
			new_vertex,
			graph_);
	}
}

void
fruitcut::sandbox::pp::system::toggle_filter(
	fcppt::string const &name)
{
	if (name_to_vertex_.find(name) == name_to_vertex_.end())
	{
		fcppt::io::cerr << FCPPT_TEXT("Warning: filter \"") << name << FCPPT_TEXT(" doesn't exist!\n");
		return;
	}

	FCPPT_ASSERT(
		vertex_to_filter_.find(name_to_vertex_[name]) != vertex_to_filter_.end());

	filter::wrapper &current_filter = vertex_to_filter_[name_to_vertex_[name]];
	current_filter.active(
		!current_filter.active());
}

void
fruitcut::sandbox::pp::system::render_scene(
	scene_render_callback const &callback)
{
	sge::renderer::scoped_target scoped_target(
		renderer_,
		screen_target_);

	sge::renderer::scoped_block scoped_block(
		renderer_);

	callback();
}

void
fruitcut::sandbox::pp::system::apply_postprocessing()
{
	// Special case: There is no filter defined (yet)
	if (name_to_vertex_.empty())
	{
		result_texture_ = screen_texture_;
		return;
	}
	
	typedef
	std::list<vertex_descriptor>
	vertex_list;

	vertex_list sorted;

	boost::topological_sort(
		graph_,
		std::front_inserter(
			sorted));

	bool scene_texture_used = false;

	typedef
	std::map
	<
		vertex_descriptor,
		sge::renderer::texture_ptr
	>
	filter_result;

	filter_result filter_result_;

	BOOST_FOREACH(
		vertex_list::const_reference current_vertex,
		sorted)
	{
		std::pair<in_edge_iterator,in_edge_iterator> ie = 
			boost::in_edges(
				current_vertex,
				graph_);

		FCPPT_ASSERT(
			vertex_to_filter_.find(current_vertex) != vertex_to_filter_.end());

		filter::wrapper const &current_filter = 
			vertex_to_filter_[current_vertex];

		try
		{
			if (ie.first == ie.second)
			{
				FCPPT_ASSERT_MESSAGE(
					!scene_texture_used,
					FCPPT_TEXT("Seems like we've got two filters without dependencies (one of them is ")+
					current_filter.name()+
					FCPPT_TEXT("). That's not possible."));

				scene_texture_used = true;

				if (!current_filter.active())
					filter_result_[current_vertex] = screen_texture_;
				else
					filter_result_[current_vertex] = 
						dynamic_cast<filter::unary &>(
							vertex_to_filter_[current_vertex].filter()).apply(
							screen_texture_);
			}
			else if (std::distance(ie.first,ie.second) == 1)
			{
				FCPPT_ASSERT(
					filter_result_.find(
						boost::source(
							*ie.first,
							graph_)) != filter_result_.end());

				if (!current_filter.active())
				{
					filter_result_[current_vertex] = 
						filter_result_[
							boost::source(
								*ie.first,
								graph_)];
				}
				else
				{
					filter_result_[current_vertex] = 
						dynamic_cast<filter::unary &>(current_filter.filter()).apply(
							filter_result_[
								boost::source(
									*ie.first,
									graph_)]);
				}
			}
			else if (std::distance(ie.first,ie.second) == 2)
			{
				vertex_descriptor const 
					v1 = 
						boost::source(
							*ie.first,
							graph_),
					v2 = 
						boost::source(
							*boost::next(
								ie.first),
							graph_);

				FCPPT_ASSERT(
					filter_result_.find(v1) != filter_result_.end() &&
					filter_result_.find(v2) != filter_result_.end());

				if (!current_filter.active())
				{
					FCPPT_ASSERT_MESSAGE(
						&(*filter_result_[v1]) == &(*filter_result_[v2]),
						FCPPT_TEXT("Encountered a deactivated binary filter ")+
						current_filter.name()+
						FCPPT_TEXT(" with two distinct dependencies. That's not possible right now"));

					filter_result_[current_vertex] = 
						filter_result_[
							v1];
				}
				else
				{
					filter_result_[current_vertex] = 
						dynamic_cast<filter::binary &>(current_filter.filter()).apply(
							filter_result_[
								v1],
							filter_result_[
								v2]);
				}
			}
			else
				throw 
					fcppt::exception(
						FCPPT_TEXT("The filter ")+
						current_filter.name()+
						FCPPT_TEXT(" has more than 3 dependencies, that's not possible"));
				
		}
		catch (std::bad_cast const &)
		{
			throw 
				fcppt::exception(
					FCPPT_TEXT("The filter ")+
					current_filter.name()+
					FCPPT_TEXT(" has more than dependencies than he can handle"));
		}
	}

	result_texture_ = 
		filter_result_[sorted.back()];
}
