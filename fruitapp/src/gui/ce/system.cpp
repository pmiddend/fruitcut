#include <fruitapp/media_path.hpp>
#include <fruitapp/gui/ce/system.hpp>
#include <fruitapp/gui/ce/dialogs/highscore.hpp>
#include <fruitapp/gui/ce/dialogs/ingame_menu.hpp>
#include <fruitapp/gui/ce/dialogs/main_menu.hpp>
#include <fruitapp/gui/ce/dialogs/name_chooser.hpp>
#include <fruitapp/gui/ce/dialogs/ranking.hpp>
#include <fruitapp/gui/ce/dialogs/settings.hpp>
#include <sge/cegui/cursor_visibility.hpp>
#include <sge/cegui/load_context.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/log/context_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::system::system(
	fcppt::log::context &_log_context,
	fruitlib::scenic::parent const &_overlay_node,
	sge::renderer::device::ffp &_renderer,
	sge::image2d::system &_image_system,
	sge::viewport::manager &_viewport_manager,
	fruitlib::scenic::delta::callback const &_standard_clock_callback,
	sge::input::focus::object &_focus,
	sge::input::cursor::object &_cursor,
	fruitlib::audio::sound_controller &_sound_controller,
	sge::renderer::texture::emulate_srgb const _emulate_srgb)
:
	focus_(
		_focus),
	cursor_(
		_cursor),
	sound_controller_(
		_sound_controller),
	overlay_node_(
		_overlay_node),
	gui_system_(
		_log_context,
		sge::cegui::load_context(
			fruitapp::media_path()/FCPPT_TEXT("gui")/FCPPT_TEXT("fruitcut.scheme"))
			.font_directory(
				fruitapp::media_path()/FCPPT_TEXT("fonts")),
		_renderer,
		_image_system,
		_viewport_manager,
		sge::cegui::cursor_visibility::invisible,
		_emulate_srgb),
	gui_syringe_(
		_log_context,
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
			std::make_unique<fruitapp::gui::ce::dialogs::highscore>(
				*this,
				_providers));
}

fruitapp::gui::dialogs::ranking_unique_ptr
fruitapp::gui::ce::system::create_ranking(
	fruitapp::highscore::provider_sequence &_providers)
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::gui::dialogs::ranking
		>(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::ranking>(
				*this,
				_providers));
}

fruitapp::gui::dialogs::main_menu_unique_ptr
fruitapp::gui::ce::system::create_main_menu()
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::gui::dialogs::main_menu
		>(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::main_menu>(
				*this));
}

fruitapp::gui::dialogs::ingame_menu_unique_ptr
fruitapp::gui::ce::system::create_ingame_menu()
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::gui::dialogs::ingame_menu
		>(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::ingame_menu>(
				*this));
}

fruitapp::gui::dialogs::settings_unique_ptr
fruitapp::gui::ce::system::create_settings(
	fruitapp::gui::initial_effects_volume const &_initial_effects_volume,
	fruitapp::gui::initial_music_volume const &_initial_music_volume,
	fruitapp::graphics_settings::object &_graphics_settings)
{
	return
		fruitapp::gui::dialogs::settings_unique_ptr(
			std::make_unique<fruitapp::gui::ce::dialogs::settings>(
				*this,
				_initial_effects_volume,
				_initial_music_volume,
				_graphics_settings));
}

fruitapp::gui::dialogs::name_chooser_unique_ptr
fruitapp::gui::ce::system::create_name_chooser(
	fruitapp::highscore::score const &_score)
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::gui::dialogs::name_chooser
		>(
			fcppt::make_unique_ptr<fruitapp::gui::ce::dialogs::name_chooser>(
				*this,
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

fruitlib::scenic::delta::callback const &
fruitapp::gui::ce::system::standard_clock_callback() const
{
	return
		standard_clock_callback_;
}

sge::input::focus::object &
fruitapp::gui::ce::system::focus() const
{
	return
		focus_;
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
