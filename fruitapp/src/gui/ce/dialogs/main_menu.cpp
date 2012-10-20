#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/main_menu.hpp>
#include <fruitapp/media_path.hpp>


fruitapp::gui::ce::dialogs::main_menu::main_menu(
	fruitapp::gui::ce::system &_system)
:
	gui_node_(
		fruitlib::scenic::optional_parent(
			_system.overlay_node()),
		_system.gui_system(),
		_system.standard_clock_callback()),
	layout_(
		_system.gui_system(),
		fruitapp::media_path()
			/ FCPPT_TEXT("gui")
			/ FCPPT_TEXT("layouts")
			/ FCPPT_TEXT("main_menu.layout")),
	gui_sheet_(
		_system.gui_system(),
		*layout_.window().getChild("MainMenu")),
	settings_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"MainMenu/Settings")),
	highscore_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"MainMenu/Highscores")),
	quit_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"MainMenu/Quit")),
	start_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"MainMenu/StartGame"))
{
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::main_menu::register_quit_callback(
	fruitapp::gui::dialogs::main_menu::quit_callback const &_f)
{
	return
		quit_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::main_menu::register_settings_callback(
	fruitapp::gui::dialogs::main_menu::settings_callback const &_f)
{
	return
		settings_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::main_menu::register_start_callback(
	fruitapp::gui::dialogs::main_menu::start_callback const &_f)
{
	return
		start_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::main_menu::register_highscore_callback(
	fruitapp::gui::dialogs::main_menu::highscore_callback const &_f)
{
	return
		highscore_button_.push_callback(
			_f);
}

fruitapp::gui::ce::dialogs::main_menu::~main_menu()
{
}
