#ifndef FRUITAPP_GUI_CE_DIALOGS_SETTINGS_HPP_INCLUDED
#define FRUITAPP_GUI_CE_DIALOGS_SETTINGS_HPP_INCLUDED

#include <fruitapp/graphics_settings/object_fwd.hpp>
#include <fruitapp/gui/initial_effects_volume.hpp>
#include <fruitapp/gui/initial_music_volume.hpp>
#include <fruitapp/gui/ce/button.hpp>
#include <fruitapp/gui/ce/combobox.hpp>
#include <fruitapp/gui/ce/system_fwd.hpp>
#include <fruitapp/gui/ce/slider/object.hpp>
#include <fruitapp/gui/dialogs/settings.hpp>
#include <fruitlib/scenic/adaptors/ce/gui_system.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace dialogs
{
class settings
:
	public fruitapp::gui::dialogs::settings
{
FCPPT_NONCOPYABLE(
	settings);
public:
	settings(
		fruitapp::gui::ce::system &,
		fruitapp::gui::initial_effects_volume const &,
		fruitapp::gui::initial_music_volume const &,
		fruitapp::graphics_settings::object &);

	fcppt::signal::auto_connection
	register_effects_volume_change_callback(
		fruitapp::gui::dialogs::settings::volume_change_callback const &);

	fcppt::signal::auto_connection
	register_music_volume_change_callback(
		fruitapp::gui::dialogs::settings::volume_change_callback const &);

	fcppt::signal::auto_connection
	register_quality_change_callback(
		fruitapp::gui::dialogs::settings::quality_change_callback const &);

	fcppt::signal::auto_connection
	register_back_callback(
		fruitapp::gui::dialogs::settings::back_callback const &);

	~settings();
private:
	typedef
	fcppt::signal::object<fruitapp::gui::dialogs::settings::volume_change_function>
	volume_change_signal;

	typedef
	fcppt::signal::object<fruitapp::gui::dialogs::settings::quality_change_function>
	quality_change_signal;

	fruitlib::scenic::adaptors::ce::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	fruitapp::gui::ce::button main_menu_button_;
	fruitapp::gui::ce::slider::object music_volume_slider_;
	fcppt::signal::scoped_connection music_volume_connection_;
	volume_change_signal music_volume_change_;
	fruitapp::gui::ce::slider::object effects_volume_slider_;
	fcppt::signal::scoped_connection effects_volume_connection_;
	volume_change_signal effects_volume_change_;
	quality_change_signal quality_change_;
	fruitapp::gui::ce::combobox quality_dropdown_;

	void
	music_volume_callback(
		fruitapp::gui::ce::slider::scalar);

	void
	effects_volume_callback(
		fruitapp::gui::ce::slider::scalar);
};
}
}
}
}

#endif
