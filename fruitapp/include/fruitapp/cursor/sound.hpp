#ifndef FRUITAPP_CURSOR_SOUND_HPP_INCLUDED
#define FRUITAPP_CURSOR_SOUND_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/input/cursor/optional_position.hpp>


namespace fruitapp
{
namespace cursor
{
class sound
:
	public fruitlib::scenic::node<sound>
{
FCPPT_NONCOPYABLE(
	sound);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	sound(
		fruitlib::scenic::optional_parent const &,
		sge::input::cursor::object &,
		fruitapp::ingame_clock const &,
		fruitapp::viewport::manager &,
		fruitlib::audio::sound_controller &);

	~sound();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::input::cursor::object &cursor_;
	fruitapp::ingame_timer update_timer_;
	fruitapp::ingame_timer cooldown_timer_;
	fruitapp::viewport::manager &viewport_manager_;
	fruitlib::audio::sound_controller &sound_controller_;
	sge::input::cursor::optional_position last_pos_;

};
}
}

#endif
