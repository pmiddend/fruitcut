#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/ranking.hpp>
#include <fruitlib/media_path.hpp>
#include <sge/cegui/from_cegui_string.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <fcppt/text.hpp>


fruitapp::gui::ce::dialogs::ranking::ranking(
	fruitapp::gui::ce::system &_system/*,
	                                    fruitapp::highscore::provider_sequence &_providers*/)
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
		fruitlib::media_path()
			/ FCPPT_TEXT("gui")
			/ FCPPT_TEXT("layouts")
			/ FCPPT_TEXT("ranking.layout")),
	gui_sheet_(
		_system.gui_system(),
		*layout_.window().getChild("Ranking")),
	highscore_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Ranking/Highscores")),
	main_menu_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Ranking/MainMenu")),
	quit_button_(
		_system.sound_controller(),
		*layout_.window().getChild(
			"Ranking/Quit"))/*,

	providers_(
		_providers) ,
	post_model_(
		providers_),
	table_view_(
		_system.charconv_system(),
		*layout_.window().getChild(
			"Ranking/List"),
			post_model_)*/
{
	/*
	post_model_.post(
		fruitapp::highscore::name(
			sge::cegui::from_cegui_string(
				context<superstate>().name(),
				context<machine>().systems().charconv_system())),
		highscore::score(
			context<machine>().last_game_score()));
	*/
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::ranking::register_highscore_callback(
	fruitapp::gui::dialogs::ranking::highscore_callback const &_f)
{
	return
		highscore_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::ranking::register_main_menu_callback(
	fruitapp::gui::dialogs::ranking::main_menu_callback const &_f)
{
	return
		main_menu_button_.push_callback(
			_f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::dialogs::ranking::register_quit_callback(
	fruitapp::gui::dialogs::ranking::quit_callback const &_f)
{
	return
		quit_button_.push_callback(
			_f);
}

void
fruitapp::gui::ce::dialogs::ranking::append_log(
	fcppt::string const &s)
{
	CEGUI::Window &w =
		*layout_.window().getChild(
			"Ranking/MessageLog");

	w.setText(
		w.getText()+
		sge::cegui::to_cegui_string(
			s,
			charconv_system_));
}

fruitapp::gui::ce::dialogs::ranking::~ranking()
{
}
