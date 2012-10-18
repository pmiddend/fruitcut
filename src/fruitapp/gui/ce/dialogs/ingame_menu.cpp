#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/ingame_menu.hpp>
#include <fruitlib/media_path.hpp>


fruitapp::gui::ce::dialogs::ingame_menu::ingame_menu(
	fruitapp::gui::ce::system &_system)
:
	gui_node_(
		fruitlib::scenic::optional_parent(
			_system.overlay_node()),
		_system.gui_system(),
		_system.standard_clock_callback()),
	gui_keyboard_(
		_system.gui_syringe(),
		_system.keyboard()),
	gui_cursor_(
		_system.gui_syringe(),
		_system.cursor()),
	layout_(
		_system.gui_system(),
		fruitlib::media_path()
			/ FCPPT_TEXT("gui")
			/ FCPPT_TEXT("layouts")
			/ FCPPT_TEXT("ingame_menu.layout")),
	gui_sheet_(
		_system.gui_system(),
		*layout_.window().getChild("MainMenu")),
	continue_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"IngameMenu/Continue")),
	main_menu_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"IngameMenu/MainMenu")),
	quit_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"IngameMenu/Quit"))
{
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::ingame_menu::register_game_callback(
	fruitapp::gui::dialogs::ingame_menu::game_callback const &_f)
{
	return
		continue_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::ingame_menu::register_main_menu_callback(
	fruitapp::gui::dialogs::ingame_menu::main_menu_callback const &_f)
{
	return
		main_menu_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::ingame_menu::register_quit_callback(
	fruitapp::gui::dialogs::ingame_menu::quit_callback const &_f)
{
	return
		quit_button_.push_callback(
			_f);
}

fruitapp::gui::ce::dialogs::ingame_menu::~ingame_menu()
{
}
