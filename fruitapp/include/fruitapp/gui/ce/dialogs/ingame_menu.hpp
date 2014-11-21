#ifndef FRUITAPP_GUI_CE_DIALOGS_INGAME_MENU_HPP_INCLUDED
#define FRUITAPP_GUI_CE_DIALOGS_INGAME_MENU_HPP_INCLUDED

#include <fruitapp/gui/ce/button.hpp>
#include <fruitapp/gui/ce/system_fwd.hpp>
#include <fruitapp/gui/dialogs/ingame_menu.hpp>
#include <fruitlib/scenic/adaptors/ce/gui_system.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace dialogs
{
class ingame_menu
:
	public fruitapp::gui::dialogs::ingame_menu
{
FCPPT_NONCOPYABLE(
	ingame_menu);
public:
	explicit
	ingame_menu(
		fruitapp::gui::ce::system &);

	fcppt::signal::auto_connection
	register_game_callback(
		fruitapp::gui::dialogs::ingame_menu::game_callback const &);

	fcppt::signal::auto_connection
	register_main_menu_callback(
		fruitapp::gui::dialogs::ingame_menu::main_menu_callback const &);

	fcppt::signal::auto_connection
	register_quit_callback(
		fruitapp::gui::dialogs::ingame_menu::quit_callback const &);

	~ingame_menu();
private:
	fruitlib::scenic::adaptors::ce::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	fruitapp::gui::ce::button continue_button_;
	fruitapp::gui::ce::button main_menu_button_;
	fruitapp::gui::ce::button quit_button_;
};
}
}
}
}

#endif
