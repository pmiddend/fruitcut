#ifndef FRUITCUT_APP_SCENE_HPP_INCLUDED
#define FRUITCUT_APP_SCENE_HPP_INCLUDED

#include "postprocessing.hpp"
#include "../fruitlib/scenic/node.hpp"
#include "../fruitlib/scenic/parent_fwd.hpp"
#include "../fruitlib/scenic/events/render_fwd.hpp"
#include "../fruitlib/scenic/events/update_fwd.hpp"
#include <boost/mpl/vector/vector10.hpp>
#include <sge/systems/instance_fwd.hpp>
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
	public fruitlib::scenic::node<scene>
{
public:
	typedef
	boost::mpl::vector2
	<
		fruitlib::scenic::events::render,
		fruitlib::scenic::events::update
	>
	scene_reactions;

	explicit
	scene(
		fruitlib::scenic::parent const &,
		sge::systems::instance const &,
		sge::parse::json::object const &);

	void
	active(
		bool);

	bool
	active() const;

	fruitcut::app::postprocessing &
	postprocessing();

	fruitcut::app::postprocessing const &
	postprocessing() const;

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	bool active_;
	fruitcut::app::postprocessing postprocessing_;

	void
	render_children();

};
}
}

#endif
