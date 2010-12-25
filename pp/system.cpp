#include "system.hpp"
#include "filter/base.hpp"
#include "filter/unary.hpp"
#include "filter/binary.hpp"
#include "filter/nullary.hpp"
#include "screen_vf/format.hpp"
#include "screen_vf/create_quad.hpp"
#include "../media_path.hpp"
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

fruitcut::pp::system::system(
	sge::renderer::device_ptr const _renderer)
:
	renderer_(
		_renderer),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>
			(sge::shader::variable(
				"target_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					renderer_->screen_size())))
			(sge::shader::variable(
				"flip",
				sge::shader::variable_type::const_,
				// bool not supported in sge
				static_cast<int>(
					0))),
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
fruitcut::pp::system::update()
{
	typedef
	std::list<vertex_descriptor>
	vertex_list;

	vertex_list sorted;

	boost::topological_sort(
		graph_,
		std::front_inserter(
			sorted));

	// No filters found...
	if (sorted.empty())
		return;

	// We store the result of the filters in this map.
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

		// This might be a bit pedantic. "sorted" is created by
		// topological_search, which should only output elements which are
		// in the graph
		FCPPT_ASSERT(
			vertex_to_filter_.find(current_vertex) != vertex_to_filter_.end());

		filter::wrapper const &current_filter = 
			vertex_to_filter_[current_vertex];

		// This try catches bad_casts from the double-dispatch
		try
		{
			if (ie.first == ie.second)
			{
				// This is a bit controversial: Should it be possible to
				// deactivate a nullary filter?
				if (!current_filter.active())
					filter_result_[current_vertex] = 
						sge::renderer::texture_ptr();
				else
					filter_result_[current_vertex] = 
						dynamic_cast<filter::nullary &>(
							vertex_to_filter_[current_vertex].filter()).apply();
			}
			else if (std::distance(ie.first,ie.second) == 1)
			{
				vertex_descriptor const 
					v1 = 
						boost::source(
							*ie.first,
							graph_);

				FCPPT_ASSERT(
					filter_result_.find(
						v1) != filter_result_.end());

				if (!current_filter.active())
				{
					filter_result_[current_vertex] = 
						filter_result_[
							v1];
				}
				else
				{
					filter_result_[current_vertex] = 
						dynamic_cast<filter::unary &>(current_filter.filter()).apply(
							filter_result_[
								v1]);
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
						FCPPT_TEXT(" with two distinct dependencies. Don't know what to do here. :("));

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

void
fruitcut::pp::system::render_result()
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
fruitcut::pp::system::add_filter(
	filter::base &b,
	fcppt::string const &name,
	dependency_set const &deps)
{
	FCPPT_ASSERT_MESSAGE(
		name_to_vertex_.find(name) == name_to_vertex_.end(),
		FCPPT_TEXT("A filter named ")+
		name+
		FCPPT_TEXT(" already exists!"));

	// - add vertex
	// - add filter wrapper for vertex
	// - add name association,
	// - add dependencies

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
			name_to_vertex_.find(r) != name_to_vertex_.end(),
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
fruitcut::pp::system::toggle_filter(
	fcppt::string const &name)
{
	if (name_to_vertex_.find(name) == name_to_vertex_.end())
	{
		fcppt::io::cerr << FCPPT_TEXT("Warning: filter \"") << name << FCPPT_TEXT(" doesn't exist!\n");
		return;
	}

	FCPPT_ASSERT(
		vertex_to_filter_.find(name_to_vertex_[name]) != vertex_to_filter_.end());

	filter::wrapper &current_filter = 
		vertex_to_filter_[name_to_vertex_[name]];
	current_filter.active(
		!current_filter.active());
}

fruitcut::pp::system::filter_name_set const
fruitcut::pp::system::filter_names() const
{
	fruitcut::pp::system::filter_name_set key_set;
	BOOST_FOREACH(
		name_to_vertex::const_reference r,
		name_to_vertex_)
		key_set.insert(
			r.first);
	return key_set;
}
