#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/name_chooser.hpp>
#include <fruitapp/media_path.hpp>
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
	continue_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Continue"))
{
	layout_.window().getChild(
		"Score")->setText(
		sge::cegui::to_cegui_string(
			fcppt::insert_to_fcppt_string(
				_score.get()),
		_system.charconv_system()));
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::name_chooser::register_continue_callback(
	fruitapp::gui::dialogs::name_chooser::continue_callback const &_continue)
{
	return
		continue_button_.push_callback(
			_continue);
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

fruitapp::gui::ce::dialogs::name_chooser::~name_chooser()
{
}
