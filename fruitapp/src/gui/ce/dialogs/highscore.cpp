#include <fruitapp/media_path.hpp>
#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/highscore.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::dialogs::highscore::highscore(
	fruitapp::gui::ce::system &_system,
	fruitapp::highscore::provider_sequence &_providers)
:
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
		_system.cursor()),
	layout_(
		_system.gui_system(),
		fruitapp::media_path()
			/ FCPPT_TEXT("gui")
			/ FCPPT_TEXT("layouts")
			/ FCPPT_TEXT("new_highscore.layout")),
	gui_sheet_(
		_system.gui_system(),
		layout_.window()),
	back_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Back")),
	providers_(
		_providers),
	source_box_(
		*layout_.window().getChild(
			"Source")),
	table_model_(),
	table_view_(
		*layout_.window().getChild(
			"List"),
		table_model_)
{
	for(
		auto const &provider
		:
		providers_
	)
		source_box_.add(
			provider->identifier(),
			std::bind(
				std::ref(
					switch_provider_),
				std::ref(
					*provider)),
			fruitapp::gui::ce::combobox::selected(
				false));
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::highscore::register_back_callback(
	fruitapp::gui::dialogs::highscore::back_callback const &_f)
{
	return
		back_button_.push_callback(
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
			_string));
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
