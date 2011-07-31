#include "filter/base.hpp"
#include "filter/manager.hpp"
#include "filter/binary.hpp"
#include "filter/nullary.hpp"
#include "filter/unary.hpp"
#include "screen_vf/format.hpp"
#include "system.hpp"
#include "texture/counted_instance.hpp"
#include "texture/instance.hpp"
#include <sge/image/color/format.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/shader/shader.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/io/cerr.hpp>
#include <boost/graph/topological_sort.hpp>
#include <iostream>
#include <iterator>
#include <list>

fruitlib::pp::system::system(
	filter::manager &_filter_manager)
:
	filter_manager_(
		_filter_manager),
	shader_(
		_filter_manager.lookup_shader(
			FCPPT_TEXT("pp_to_screen"),
			sge::shader::variable_sequence(),
			fcppt::assign::make_container<sge::shader::sampler_sequence>(
				sge::shader::sampler(
					"tex",
					sge::renderer::texture::planar_ptr()))))
{
}

void
fruitlib::pp::system::update()
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
	
	for(
		vertex_list::const_iterator current_vertex = 
			sorted.begin();
		current_vertex != sorted.end();
		++current_vertex)
	{
		texture::counted_instance const result = 
			vertex_to_filter_[*current_vertex].filter().dispatch();

		result_texture_ = result->texture();
		for(
			std::pair<out_edge_iterator,out_edge_iterator> out_edge_pair = 
				boost::out_edges(
					*current_vertex,
					graph_);
			out_edge_pair.first != out_edge_pair.second;
			++out_edge_pair.first)
		{
			vertex_to_filter_[boost::target(*out_edge_pair.first,graph_)].filter().enqueue(
				result);
		}
	}

	/*
	result_texture_ = 
		vertex_to_filter_[sorted.back()].filter().textures_.front()->texture();
	*/
}

sge::renderer::texture::planar_ptr const
fruitlib::pp::system::result_texture()
{
	return result_texture_;
}

void
fruitlib::pp::system::render_result()
{
	shader_.update_texture(
		"tex",
		result_texture_);

	sge::shader::scoped scoped_shader(
		shader_,
		sge::shader::activate_everything());

	filter_manager_.quad().render();
}

void
fruitlib::pp::system::add_filter(
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

	for(
		dependency_set::const_iterator r = 
			deps.begin();
		r != deps.end();
		++r)
	{
		FCPPT_ASSERT_MESSAGE(
			name_to_vertex_.find(*r) != name_to_vertex_.end(),
			FCPPT_TEXT("Filter ")+
			(*r)+
			FCPPT_TEXT(" which was specified as the dependency for ")+
			name+
			FCPPT_TEXT(" was not found"));
			
		boost::add_edge(
			name_to_vertex_[(*r)],
			new_vertex,
			graph_);
	}
}

void
fruitlib::pp::system::toggle_filter(
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

fruitlib::pp::system::filter_name_set const
fruitlib::pp::system::filter_names() const
{
	fruitlib::pp::system::filter_name_set key_set;
	for(name_to_vertex::const_iterator r = name_to_vertex_.begin(); r != name_to_vertex_.end(); ++r)
		key_set.insert(
			r->first);
	return key_set;
}
