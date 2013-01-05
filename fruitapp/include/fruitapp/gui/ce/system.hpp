#ifndef FRUITAPP_GUI_CE_SYSTEM_HPP_INCLUDED
#define FRUITAPP_GUI_CE_SYSTEM_HPP_INCLUDED

#include <fruitapp/gui/system.hpp>
#include <fruitlib/audio/sound_controller_fwd.hpp>
#include <fruitlib/scenic/parent.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <sge/cegui/syringe.hpp>
#include <sge/cegui/syringe_fwd.hpp>
#include <sge/cegui/system.hpp>
#include <sge/cegui/system_fwd.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/texture/emulate_srgb_fwd.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
class system
:
	public fruitapp::gui::system
{
FCPPT_NONCOPYABLE(
	system);
public:
	system(
		fruitlib::scenic::parent const &,
		sge::renderer::device::ffp &,
		sge::image2d::system &,
		sge::viewport::manager &,
		sge::charconv::system &,
		fruitlib::scenic::delta::callback const &,
		sge::input::keyboard::device &,
		sge::input::cursor::object &,
		fruitlib::audio::sound_controller &,
		sge::renderer::texture::emulate_srgb);

	fruitapp::gui::dialogs::highscore_unique_ptr
	create_highscore(
		fruitapp::highscore::provider_sequence &);

	fruitapp::gui::dialogs::ranking_unique_ptr
	create_ranking(
		fruitapp::highscore::provider_sequence &);

	fruitapp::gui::dialogs::main_menu_unique_ptr
	create_main_menu();

	fruitapp::gui::dialogs::ingame_menu_unique_ptr
	create_ingame_menu();

	fruitapp::gui::dialogs::settings_unique_ptr
	create_settings(
		fruitapp::gui::initial_effects_volume const &,
		fruitapp::gui::initial_music_volume const &,
		fruitapp::graphics_settings::object &);

	fruitapp::gui::dialogs::name_chooser_unique_ptr
	create_name_chooser(
		fruitapp::highscore::score const &);

	fruitlib::scenic::parent const &
	overlay_node() const;

	sge::cegui::system &
	gui_system();

	sge::cegui::syringe &
	gui_syringe();

	sge::charconv::system &
	charconv_system() const;

	fruitlib::scenic::delta::callback const &
	standard_clock_callback() const;

	sge::input::keyboard::device &
	keyboard() const;

	sge::input::cursor::object &
	cursor() const;

	fruitlib::audio::sound_controller &
	sound_controller() const;

	~system();
private:
	sge::charconv::system &charconv_system_;
	sge::input::keyboard::device &keyboard_;
	sge::input::cursor::object &cursor_;
	fruitlib::audio::sound_controller &sound_controller_;
	fruitlib::scenic::parent overlay_node_;
	sge::cegui::system gui_system_;
	sge::cegui::syringe gui_syringe_;
	fruitlib::scenic::delta::callback standard_clock_callback_;
};
}
}
}

#endif
