#ifndef FRUITCUT_APP_RENDERABLE_HPP_INCLUDED
#define FRUITCUT_APP_RENDERABLE_HPP_INCLUDED

#include "scene.hpp"
#include "overlay.hpp"
#include "postprocessing.hpp"
#include "../fruitlib/scenic/node.hpp"
#include "../fruitlib/scenic/parent_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>

namespace fruitcut
{
namespace app
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
	- The "overlay" node is updated. This first updates the nodes
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
public:
	explicit
	renderable(
		fruitlib::scenic::parent const &,
		sge::systems::instance const &,
		sge::parse::json::object const &);

	app::scene &
	scene();

	app::scene const &
	scene() const;

	app::overlay &
	overlay();

	app::overlay const &
	overlay() const;

	app::postprocessing &
	postprocessing();

	app::postprocessing const &
	postprocessing() const;

	~renderable();
private:
	app::scene scene_;
	app::postprocessing postprocessing_;
	app::overlay overlay_;
};
}
}

#endif
