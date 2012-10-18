#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/highscore.hpp>
#include <fruitapp/gui/ce/dialogs/ingame_menu.hpp>
#include <fruitapp/gui/ce/dialogs/main_menu.hpp>
#include <fruitapp/gui/ce/dialogs/name_chooser.hpp>
#include <fruitapp/gui/ce/dialogs/ranking.hpp>
#include <fruitapp/gui/ce/dialogs/settings.hpp>
#include <fruitlib/media_path.hpp>
#include <sge/cegui/load_context.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>


fruitapp::gui::ce::system::system(
	fruitlib::scenic::parent const &_overlay_node,
	sge::renderer::device::ffp &_renderer,
	sge::image2d::system &_image_system,
	sge::viewport::manager &_viewport_manager,
	sge::charconv::system &_charconv_system,
	fruitlib::scenic::delta::callback const &_standard_clock_callback,
	sge::input::keyboard::device &_keyboard,
	sge::input::cursor::object &_cursor,
	fruitlib::audio::sound_controller &_sound_controller)
:
	charconv_system_(
		_charconv_system),
	keyboard_(
		_keyboard),
	cursor_(
		_cursor),
	sound_controller_(
		_sound_controller),
	overlay_node_(
		_overlay_node),
	gui_system_(
		sge::cegui::load_context(
			fruitlib::media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("fruitcut.scheme"))
			.font_directory(
				fruitlib::media_path()/FCPPT_TEXT("fonts")),
		_renderer,
		_image_system,
		charconv_system_,
		_viewport_manager,
		sge::cegui::cursor_visibility::invisible),
	gui_syringe_(
		gui_system_),
	standard_clock_callback_(
		_standard_clock_callback)
{
}

fruitapp::gui::dialogs::highscore_unique_ptr
fruitapp::gui::ce::system::create_highscore(
	  fruitapp::highscore::provider_sequence &_providers)
{
	return
		fruitapp::gui::dialogs::highscore_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::highscore>(
				fcppt::ref(
					*this),
				fcppt::ref(
					_providers)));
}

fruitapp::gui::dialogs::ranking_unique_ptr
fruitapp::gui::ce::system::create_ranking(
	/*fruitapp::highscore::provider_sequence &_providers*/)
{
	return
		fruitapp::gui::dialogs::ranking_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::ranking>(
				fcppt::ref(
					*this)/*,
				fcppt::ref(
				_providers)*/));
}

fruitapp::gui::dialogs::main_menu_unique_ptr
fruitapp::gui::ce::system::create_main_menu()
{
	return
		fruitapp::gui::dialogs::main_menu_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::main_menu>(
				fcppt::ref(
					*this)));
}

fruitapp::gui::dialogs::ingame_menu_unique_ptr
fruitapp::gui::ce::system::create_ingame_menu()
{
	return
		fruitapp::gui::dialogs::ingame_menu_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::ingame_menu>(
				fcppt::ref(
					*this)));
}

fruitapp::gui::dialogs::settings_unique_ptr
fruitapp::gui::ce::system::create_settings(
	fruitapp::gui::initial_effects_volume const &_initial_effects_volume,
	fruitapp::gui::initial_music_volume const &_initial_music_volume)
{
	return
		fruitapp::gui::dialogs::settings_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::settings>(
				fcppt::ref(
					*this),
				_initial_effects_volume,
				_initial_music_volume));
}

fruitapp::gui::dialogs::name_chooser_unique_ptr
fruitapp::gui::ce::system::create_name_chooser(
	fruitapp::highscore::score const &_score)
{
	return
		fruitapp::gui::dialogs::name_chooser_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::name_chooser>(
				fcppt::ref(
					*this),
				_score));
}

fruitlib::scenic::parent const &
fruitapp::gui::ce::system::overlay_node() const
{
	return
		overlay_node_;
}

sge::cegui::system &
fruitapp::gui::ce::system::gui_system()
{
	return
		gui_system_;
}

sge::cegui::syringe &
fruitapp::gui::ce::system::gui_syringe()
{
	return
		gui_syringe_;
}

sge::charconv::system &
fruitapp::gui::ce::system::charconv_system() const
{
	return
		charconv_system_;
}

fruitlib::scenic::delta::callback const &
fruitapp::gui::ce::system::standard_clock_callback() const
{
	return
		standard_clock_callback_;
}

sge::input::keyboard::device &
fruitapp::gui::ce::system::keyboard() const
{
	return
		keyboard_;
}

sge::input::cursor::object &
fruitapp::gui::ce::system::cursor() const
{
	return
		cursor_;
}

fruitlib::audio::sound_controller &
fruitapp::gui::ce::system::sound_controller() const
{
	return
		sound_controller_;
}

fruitapp::gui::ce::system::~system()
{
}
