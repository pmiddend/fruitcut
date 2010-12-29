#ifndef FRUITCUT_PP_SYSTEM_HPP_INCLUDED
#define FRUITCUT_PP_SYSTEM_HPP_INCLUDED

#include "filter/base_fwd.hpp"
#include "filter/wrapper.hpp"
#include "dependency_set.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/unordered/unordered_set.hpp>
#include <map>

namespace fruitcut
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
 */
class system
{
FCPPT_NONCOPYABLE(system)
public:
	typedef
	boost::unordered_set<fcppt::string>
	filter_name_set;

	explicit
	system(
		sge::renderer::device_ptr);

	void
	update();

	void
	render_result();

	void
	add_filter(
		filter::base &,
		fcppt::string const &name,
		dependency_set const &);

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
	std::map
	<
		vertex_descriptor,
		filter::wrapper
	>
	vertex_to_filter;

	typedef
	boost::unordered_map
	<
		fcppt::string,
		vertex_descriptor
	>
	name_to_vertex;

	sge::renderer::device_ptr const renderer_;
	graph graph_;
	vertex_to_filter vertex_to_filter_;
	name_to_vertex name_to_vertex_;
	sge::shader::object shader_;
	sge::renderer::vertex_buffer_ptr quad_;
	sge::renderer::texture_ptr result_texture_;
};
}
}

#endif
