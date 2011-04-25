#ifndef FRUITCUT_APP_SCENE_HPP_INCLUDED
#define FRUITCUT_APP_SCENE_HPP_INCLUDED

#include "postprocessing.hpp"
#include "../scenic/nodes/intrusive_group.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/console/object_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>

namespace fruitcut
{
namespace app
{
// There might be a better solution to the scene's activation: We
// could add a new scenic node "activatable" which would become
// scene's parent and would forward "update" and "render" to its
// children if active_ is true.
class scene
:
	public scenic::nodes::intrusive_group
{
public:
	explicit
	scene(
		sge::systems::instance const &,
		sge::console::object &,
		sge::parse::json::object const &);

	void
	active(
		bool);

	bool
	active() const;

	void
	render();

	void 
	update();

	fruitcut::app::postprocessing &
	postprocessing();

	fruitcut::app::postprocessing const &
	postprocessing() const;
private:
	bool active_;
	fruitcut::app::postprocessing postprocessing_;

	void
	render_children();
};
}
}

#endif
