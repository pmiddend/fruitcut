#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/highscore.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fruitapp/media_path.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::gui::ce::dialogs::highscore::highscore(
	fruitapp::gui::ce::system &_system,
	fruitapp::highscore::provider_sequence &_providers)
:
	charconv_system_(
		_system.charconv_system()),
	switch_provider_(),
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
		_system.cursor(),
		_system.mouse()),
	layout_(
		_system.gui_system(),
		fruitapp::media_path()
			/ FCPPT_TEXT("gui")
			/ FCPPT_TEXT("layouts")
			/ FCPPT_TEXT("new_highscore.layout")),
	gui_sheet_(
		_system.gui_system(),
		layout_.window()),
	main_menu_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"MainMenu")),
	quit_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Quit")),
	providers_(
		_providers),
	source_box_(
		*layout_.window().getChild(
			"Source")),
	table_model_(),
	table_view_(
		_system.charconv_system(),
		*layout_.window().getChild(
			"List"),
		table_model_)
{
	for(
		fruitapp::highscore::provider_sequence::iterator i =
			providers_.begin();
		i != providers_.end();
		++i)
		source_box_.add(
			fcppt::to_std_string(
				i->identifier()),
			std::tr1::bind(
				std::tr1::ref(
					switch_provider_),
				fcppt::ref(
					*i)));
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::highscore::register_back_callback(
	fruitapp::gui::dialogs::highscore::back_callback const &_f)
{
	return
		main_menu_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::highscore::register_quit_callback(
	fruitapp::gui::dialogs::highscore::quit_callback const &_f)
{
	return
		quit_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::highscore::register_switch_provider_callback(
	fruitapp::gui::dialogs::highscore::switch_provider_callback const &_switch_provider)
{
	return
		switch_provider_.connect(
			_switch_provider);
}

void
fruitapp::gui::ce::dialogs::highscore::clear_log()
{
	layout_.window().getChild("CaptionedStaticText")->getChild("MessageLog")->setText("");
}

void
fruitapp::gui::ce::dialogs::highscore::append_log(
	fcppt::string const &_string)
{
	CEGUI::Window &w =
		*(layout_.window().getChild("CaptionedStaticText")->getChild("MessageLog"));

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			_string,
			charconv_system_));
}

void
fruitapp::gui::ce::dialogs::highscore::process_list(
	fruitapp::highscore::entry_set const &_entries)
{
	table_model_.reset(
		_entries);
}

fruitapp::gui::ce::dialogs::highscore::~highscore()
{
}
