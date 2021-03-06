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
#include <sge/image2d/system_fwd.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/input/focus/object_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/texture/emulate_srgb_fwd.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/log/context_fwd.hpp>


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
		fcppt::log::context &,
		fruitlib::scenic::parent const &,
		sge::renderer::device::ffp &,
		sge::image2d::system &,
		sge::viewport::manager &,
		fruitlib::scenic::delta::callback const &,
		sge::input::focus::object &,
		sge::input::cursor::object &,
		fruitlib::audio::sound_controller &,
		sge::renderer::texture::emulate_srgb);

	fruitapp::gui::dialogs::highscore_unique_ptr
	create_highscore(
		fruitapp::highscore::provider_sequence &
	)
	override;

	fruitapp::gui::dialogs::ranking_unique_ptr
	create_ranking(
		fruitapp::highscore::provider_sequence &
	)
	override;

	fruitapp::gui::dialogs::main_menu_unique_ptr
	create_main_menu()
	override;

	fruitapp::gui::dialogs::ingame_menu_unique_ptr
	create_ingame_menu()
	override;

	fruitapp::gui::dialogs::settings_unique_ptr
	create_settings(
		fruitapp::gui::initial_effects_volume const &,
		fruitapp::gui::initial_music_volume const &,
		fruitapp::graphics_settings::object &
	)
	override;

	fruitapp::gui::dialogs::name_chooser_unique_ptr
	create_name_chooser(
		fruitapp::highscore::score const &
	)
	override;

	fruitlib::scenic::parent const &
	overlay_node() const;

	sge::cegui::system &
	gui_system();

	sge::cegui::syringe &
	gui_syringe();

	fruitlib::scenic::delta::callback const &
	standard_clock_callback() const;

	sge::input::focus::object &
	focus() const;

	sge::input::cursor::object &
	cursor() const;

	fruitlib::audio::sound_controller &
	sound_controller() const;

	~system()
	override;
private:
	sge::input::focus::object &focus_;
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
