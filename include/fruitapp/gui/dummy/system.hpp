#ifndef FRUITAPP_GUI_DUMMY_SYSTEM_HPP_INCLUDED
#define FRUITAPP_GUI_DUMMY_SYSTEM_HPP_INCLUDED

#include <fruitapp/gui/system.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace gui
{
namespace dummy
{
class system
:
	public fruitapp::gui::system
{
FCPPT_NONCOPYABLE(
	system);
public:
	explicit
	system(
		sge::input::keyboard::device &);

	fruitapp::gui::dialogs::highscore_unique_ptr
	create_highscore(
		fruitapp::highscore::provider_sequence &);

	fruitapp::gui::dialogs::ranking_unique_ptr
	create_ranking();

	fruitapp::gui::dialogs::main_menu_unique_ptr
	create_main_menu();

	fruitapp::gui::dialogs::ingame_menu_unique_ptr
	create_ingame_menu();

	fruitapp::gui::dialogs::settings_unique_ptr
	create_settings(
		fruitapp::gui::initial_effects_volume const &,
		fruitapp::gui::initial_music_volume const &);

	fruitapp::gui::dialogs::name_chooser_unique_ptr
	create_name_chooser(
		fruitapp::highscore::score const &);

	~system();
private:
	sge::input::keyboard::device &keyboard_;
};
}
}
}

#endif
