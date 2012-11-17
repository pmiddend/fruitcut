#include <fruitapp/media_path.hpp>
#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/name_chooser.hpp>
#include <sge/cegui/from_cegui_string.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>


fruitapp::gui::ce::dialogs::name_chooser::name_chooser(
	fruitapp::gui::ce::system &_system,
	fruitapp::highscore::score const &_score)
:
	charconv_system_(
		_system.charconv_system()),
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
		fruitapp::media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("layouts")/FCPPT_TEXT("name_chooser.layout")),
	gui_sheet_(
		_system.gui_system(),
		layout_.window()),
	submit_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Submit")),
	main_menu_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"MainMenu")),
	restart_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Restart"))
{
	layout_.window().getChild("ScoreFrame")->getChild("Score")->setText(
		sge::cegui::to_cegui_string(
			fcppt::insert_to_fcppt_string(
				_score.get()),
		_system.charconv_system()));
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::name_chooser::register_submit_callback(
	fruitapp::gui::dialogs::name_chooser::submit_callback const &_submit)
{
	return
		submit_button_.push_callback(
			_submit);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::name_chooser::register_main_menu_callback(
	fruitapp::gui::dialogs::name_chooser::main_menu_callback const &_main_menu)
{
	return
		main_menu_button_.push_callback(
			_main_menu);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::name_chooser::register_restart_callback(
	fruitapp::gui::dialogs::name_chooser::restart_callback const &_restart)
{
	return
		restart_button_.push_callback(
			_restart);
}

fcppt::string const
fruitapp::gui::ce::dialogs::name_chooser::name() const
{
	return
		sge::cegui::from_cegui_string(
			layout_.window().getChild(
				"Name")->getText(),
			charconv_system_);
}

void
fruitapp::gui::ce::dialogs::name_chooser::name(
	fcppt::string const &_name)
{
	layout_.window().getChild(
		"Name")->setText(
			sge::cegui::to_cegui_string(
				_name,
				charconv_system_));
}

fruitapp::gui::ce::dialogs::name_chooser::~name_chooser()
{
}
