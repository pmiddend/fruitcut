#include <fruitlib/audio/sound_controller.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <sge/input/cursor/object.hpp>
#include <sge/renderer/device.hpp>

namespace fruitapp
{
class cursor_sound
:
	public fruitlib::scenic::node<cursor_sound>
{
FCPPT_NONCOPYABLE(
	cursor_sound);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	cursor_sound(
		fruitlib::scenic::optional_parent const &,
		sge::input::cursor::object &,
		fruitapp::ingame_clock const &,
		sge::renderer::device &,
		fruitlib::audio::sound_controller &);

	~cursor_sound();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::input::cursor::object &cursor_;
	fruitapp::ingame_timer update_timer_;
	fruitapp::ingame_timer cooldown_timer_;
	sge::renderer::device &renderer_;
	fruitlib::audio::sound_controller &sound_controller_;
	sge::input::cursor::position last_pos_;

};
}

