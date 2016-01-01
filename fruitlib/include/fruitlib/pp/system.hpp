#ifndef FRUITLIB_PP_SYSTEM_HPP_INCLUDED
#define FRUITLIB_PP_SYSTEM_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/pp/dependency_set.hpp>
#include <fruitlib/pp/filter/base_fwd.hpp>
#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/filter/wrapper.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <sge/renderer/context/core_fwd.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/planar_texture.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <map>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace pp
{
/**
	All that is in 'pp' represents some kind of postprocessing. In
	postprocessing, there are one or more texture "sources" and
	(currently) exactly one texture sink. Every filter inbetween somehow
	combines the different sources to a resulting image (this happens in
	update()), which can then be drawn on the screen with
	render_result().

	The concept is based on a filter graph. Filters are functions getting
	zero or more textures as argument and returning another texture as
	output. A nullary filter is a source, because it gets no input and
	returns a texture. There are unary and binary filters, too.

	Currently, "render_to_texture" is the only nullary filter. It gets a
	render callback and calls this to render everything to a texture of
	the specified size.

	Internally, a graph is built with the filters as nodes and
	dependencies as the edges. When update is called, the graph is
	sorted topologically. Then, in this topological order, all nodes are
	visited and filter::base::dispatch is called on them. This function
	analyzes the internal texture queue.

	The size of this queue determines the arity of the filter. If it
	contains no textures, dispatch casts to "nullary" and passes no
	arguments. If it contains one texture, a cast to "unary" is made and
	the single texture is passed, and so on. The internal queue is
	emptied after the dispatch() call, which might lead to an unlock on
	a texture which is now not referenced from any filter anymore.

	The result from the dispatch call is put into the internal queues of
	the successor filters, so the process can be repeated with those
	nodes.

	A filter can request a texture with a target attached from the
	texture_manager. The texture_manager checks if a texture with the
	desired size/filter and color format already exists and is not
	locked. If none exists, a new one is created.
 */
class system
{
FCPPT_NONCOPYABLE(
	system);
public:
	typedef
	boost::unordered_set<fcppt::string>
	filter_name_set;

	FRUITLIB_DETAIL_SYMBOL
	explicit
	system(
		fruitlib::pp::filter::manager &);

	FRUITLIB_DETAIL_SYMBOL
	void
	update();

	FRUITLIB_DETAIL_SYMBOL
	fruitlib::pp::texture::counted_instance const
	result_texture();

	FRUITLIB_DETAIL_SYMBOL
	void
	render_result(
		sge::renderer::context::core &);

	FRUITLIB_DETAIL_SYMBOL
	void
	add_filter(
		fruitlib::pp::filter::base &,
		fcppt::string const &name,
		fruitlib::pp::dependency_set const &);

	void
	toggle_filter(
		fcppt::string const &name);

	filter_name_set const
	filter_names() const;
private:
	typedef
	boost::adjacency_list
	<
		boost::vecS,
		boost::vecS,
		boost::bidirectionalS
	>
	graph;

	typedef
	boost::graph_traits<graph>::vertex_descriptor
	vertex_descriptor;

	typedef
	boost::graph_traits<graph>::edge_descriptor
	edge_descriptor;

	typedef
	boost::graph_traits<graph>::in_edge_iterator
	in_edge_iterator;

	typedef
	boost::graph_traits<graph>::out_edge_iterator
	out_edge_iterator;

	typedef
	std::map
	<
		vertex_descriptor,
		fruitlib::pp::filter::wrapper
	>
	vertex_to_filter;

	typedef
	boost::unordered_map
	<
		fcppt::string,
		vertex_descriptor
	>
	name_to_vertex;

	filter::manager &filter_manager_;
	graph graph_;
	vertex_to_filter vertex_to_filter_;
	name_to_vertex name_to_vertex_;
	sge::shader::pair shader_;
	sge::shader::parameter::planar_texture texture_parameter_;
	fruitlib::pp::texture::counted_instance result_texture_;
};
}
}

#endif
