#ifndef FRUITAPP_RENDERABLE_HPP_INCLUDED
#define FRUITAPP_RENDERABLE_HPP_INCLUDED

#include <fruitapp/overlay.hpp>
#include <fruitapp/postprocessing.hpp>
#include <fruitapp/scene.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/systems/instance_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
/**
	How fruitcut renders stuff:

	This class "renderable" is responsible for the process. It sits
	somewhere below the root node of the scene graph. When given an
	update event, the following happens:

	- The "scene" node below this node is updated if it is activated
    (nothing special happens here)
	- The "postprocessing" node is updated. In the update function, the
    postprocessing graph begins rendering (renderer->begin_rendering)
    and calls a callback function inside the scoped render block.
	- This callback function triggers scene::render_children.
	- scene::render_children sends a render event to its children
	- The rendering output isn't displayed, but saved in a texture.
	- The "overlay" node is updated. This first updates the node's
    children (forwards the update event)
	- Then, still as a reaction to the update event, the overlay begins
    a new scoped render block, this time rendering directly to the
    framebuffer.
	- The overlay calls "postprocessing::render_result" to draw the
    texture produced by the scene node
	- The overlay then renders its children.
	*/
class renderable
:
	public fruitlib::scenic::node<renderable>
{
FCPPT_NONCOPYABLE(
	renderable);
public:
	explicit
	renderable(
		fruitlib::scenic::optional_parent const &,
		sge::systems::instance const &,
		sge::parse::json::object const &);

	fruitapp::scene &
	scene();

	fruitapp::scene const &
	scene() const;

	fruitapp::overlay &
	overlay();

	fruitapp::overlay const &
	overlay() const;

	fruitapp::postprocessing &
	postprocessing();

	fruitapp::postprocessing const &
	postprocessing() const;

	~renderable();
private:
	fruitapp::scene scene_;
	fruitapp::postprocessing postprocessing_;
	fruitapp::overlay overlay_;
};
}

#endif
