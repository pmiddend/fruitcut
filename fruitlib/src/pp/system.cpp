#include <fruitlib/pp/system.hpp>
#include <fruitlib/pp/filter/base.hpp>
#include <fruitlib/pp/filter/binary.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/filter/nullary.hpp>
#include <fruitlib/pp/filter/unary.hpp>
#include <fruitlib/pp/screen_vf/format.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/assert/pre_message.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/graph/topological_sort.hpp>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <fcppt/config/external_end.hpp>


namespace
{
char const pp_to_screen_source[] =
	"struct vertex_outputs\n"
	"{\n"
	"	float4 position : POSITION;\n"
	"	float2 texture_coordinate : TEXCOORD0;\n"
	"};\n"
	"vertex_outputs\n"
	"vertex_main(\n"
	"	in float2 position : POSITION)\n"
	"{\n"
	"	vertex_outputs outs;\n"
	"	outs.position = float4(position.xy,0.0,1.0);\n"
	"	outs.texture_coordinate = (position.xy + float2(1.0,1.0))/2.0;\n"
	"	return\n"
	"		outs;\n"
	"}\n"
	"float4\n"
	"pixel_main(\n"
	"	vertex_outputs vertex_data,\n"
	"	uniform sampler2D input_texture)\n"
	"	: COLOR\n"
	"{\n"
	"	return\n"
	"		tex2D(\n"
	"			  input_texture,\n"
	"			  vertex_data.texture_coordinate);\n"
	"}\n";
}

fruitlib::pp::system::system(
	fruitlib::pp::filter::manager &_filter_manager)
:
	filter_manager_(
		_filter_manager),
	graph_(),
	vertex_to_filter_(),
	name_to_vertex_(),
	shader_(
		_filter_manager.shader_context(),
		_filter_manager.quad().vertex_declaration(),
		sge::shader::vertex_program_stream(
			*fcppt::make_unique_ptr<std::istringstream>(
				std::string(
					pp_to_screen_source))),
		sge::shader::pixel_program_stream(
			*fcppt::make_unique_ptr<std::istringstream>(
				std::string(
					pp_to_screen_source))),
		_filter_manager.shader_cflags()),
	texture_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture"),
		shader_,
		filter_manager_.renderer(),
		sge::shader::parameter::planar_texture::optional_value()),
	result_texture_()
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
		result_texture_ =
			vertex_to_filter_[*current_vertex].filter().dispatch();

		for(
			std::pair<out_edge_iterator,out_edge_iterator> out_edge_pair =
				boost::out_edges(
					*current_vertex,
					graph_);
			out_edge_pair.first != out_edge_pair.second;
			++out_edge_pair.first)
		{
			vertex_to_filter_[boost::target(*out_edge_pair.first,graph_)].filter().enqueue(
				result_texture_);
		}
	}
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::system::result_texture()
{
	return
		result_texture_;
}

void
fruitlib::pp::system::render_result(
	sge::renderer::context::core &_context)
{
	texture_parameter_.set(
		sge::shader::parameter::planar_texture::optional_value(
			*(result_texture_->texture())));

	sge::shader::scoped_pair scoped_shader(
		_context,
		shader_);

	filter_manager_.quad().render(
		_context);
}

void
fruitlib::pp::system::add_filter(
	filter::base &b,
	fcppt::string const &name,
	dependency_set const &deps)
{
	FCPPT_ASSERT_PRE_MESSAGE(
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
		FCPPT_ASSERT_PRE_MESSAGE(
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
		fcppt::io::cerr() << FCPPT_TEXT("Warning: filter \"") << name << FCPPT_TEXT(" doesn't exist!\n");
		return;
	}

	FCPPT_ASSERT_PRE(
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
