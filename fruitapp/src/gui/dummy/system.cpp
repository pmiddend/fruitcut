#include <fruitapp/gui/dialogs/highscore.hpp>
#include <fruitapp/gui/dialogs/settings.hpp>
#include <fruitapp/gui/dummy/system.hpp>
#include <fruitapp/gui/dummy/dialogs/ingame_menu.hpp>
#include <fruitapp/gui/dummy/dialogs/main_menu.hpp>
#include <fruitapp/gui/dummy/dialogs/name_chooser.hpp>
#include <fruitapp/gui/dummy/dialogs/ranking.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>


fruitapp::gui::dummy::system::system(
	sge::input::keyboard::device &_keyboard)
:
	keyboard_(
		_keyboard)
{
}

fruitapp::gui::dialogs::highscore_unique_ptr
fruitapp::gui::dummy::system::create_highscore(
	fruitapp::highscore::provider_sequence &)
{
	return
		fruitapp::gui::dialogs::highscore_unique_ptr();
}

fruitapp::gui::dialogs::ranking_unique_ptr
fruitapp::gui::dummy::system::create_ranking(
	fruitapp::highscore::provider_sequence &)
{
	return
		fruitapp::gui::dialogs::ranking_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::dummy::dialogs::ranking>(
				fcppt::ref(
					keyboard_)));
}

fruitapp::gui::dialogs::main_menu_unique_ptr
fruitapp::gui::dummy::system::create_main_menu()
{
	return
		fruitapp::gui::dialogs::main_menu_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::dummy::dialogs::main_menu>(
				fcppt::ref(
					keyboard_)));
}

fruitapp::gui::dialogs::ingame_menu_unique_ptr
fruitapp::gui::dummy::system::create_ingame_menu()
{
	return
		fruitapp::gui::dialogs::ingame_menu_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::dummy::dialogs::ingame_menu>(
				fcppt::ref(
					keyboard_)));
}

fruitapp::gui::dialogs::settings_unique_ptr
fruitapp::gui::dummy::system::create_settings(
	fruitapp::gui::initial_effects_volume const &,
	fruitapp::gui::initial_music_volume const &)
{
	return
		fruitapp::gui::dialogs::settings_unique_ptr();
}

fruitapp::gui::dialogs::name_chooser_unique_ptr
fruitapp::gui::dummy::system::create_name_chooser(
	fruitapp::highscore::score const &)
{
	return
		fruitapp::gui::dialogs::name_chooser_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::dummy::dialogs::name_chooser>(
				fcppt::ref(
					keyboard_)));
}

fruitapp::gui::dummy::system::~system()
{
}
