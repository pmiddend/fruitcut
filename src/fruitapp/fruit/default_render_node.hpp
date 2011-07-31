#ifndef FRUITAPP_FRUIT_DEFAULT_RENDER_NODE_HPP_INCLUDED
#define FRUITAPP_FRUIT_DEFAULT_RENDER_NODE_HPP_INCLUDED

#include "manager_fwd.hpp"
#include <fruitapp/directional_light_source_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/parent_fwd.hpp>
#include <fruitlib/scenic/events/render_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
namespace fruit
{
class default_render_node
:
	public fruitlib::scenic::node<default_render_node>
{
FCPPT_NONCOPYABLE(
	default_render_node);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	explicit
	default_render_node(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device &,
		sge::renderer::vertex_declaration &,
		fruit::manager const &,
		sge::camera::object &,
		fruitapp::directional_light_source const &,
		sge::renderer::scalar);

	~default_render_node();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	sge::renderer::device &renderer_;
	fruit::manager const &manager_;
	sge::camera::object const &camera_;
	sge::shader::object shader_;
};
}
}

#endif
