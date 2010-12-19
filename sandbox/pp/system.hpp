#ifndef FRUITCUT_SANDBOX_PP_SYSTEM_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_SYSTEM_HPP_INCLUDED

#include "filter/base_fwd.hpp"
#include "filter/wrapper.hpp"
#include "scene_render_callback.hpp"
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
#include <map>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
class system
{
FCPPT_NONCOPYABLE(system)
public:
	explicit
	system(
		sge::renderer::device_ptr);

	void
	update(
		scene_render_callback const &);

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
	sge::renderer::texture_ptr const screen_texture_;
	sge::renderer::target_ptr const screen_target_;
	graph graph_;
	vertex_to_filter vertex_to_filter_;
	name_to_vertex name_to_vertex_;
	sge::shader::object shader_;
	sge::renderer::vertex_buffer_ptr quad_;
	sge::renderer::texture_ptr result_texture_;

	void
	render_scene(
		scene_render_callback const &);

	void
	apply_postprocessing();
};
}
}
}

#endif
