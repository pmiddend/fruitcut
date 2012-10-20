#ifndef FRUITAPP_GUI_CE_DIALOGS_MAIN_MENU_HPP_INCLUDED
#define FRUITAPP_GUI_CE_DIALOGS_MAIN_MENU_HPP_INCLUDED

#include <fruitapp/gui/ce/button.hpp>
#include <fruitapp/gui/ce/system_fwd.hpp>
#include <fruitapp/gui/dialogs/main_menu.hpp>
#include <fruitlib/scenic/adaptors/gui_system.hpp>
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
class main_menu
:
	public fruitapp::gui::dialogs::main_menu
{
FCPPT_NONCOPYABLE(
	main_menu);
public:
	explicit
	main_menu(
		fruitapp::gui::ce::system &);

	fcppt::signal::auto_connection
	register_quit_callback(
		fruitapp::gui::dialogs::main_menu::quit_callback const &);

	fcppt::signal::auto_connection
	register_settings_callback(
		fruitapp::gui::dialogs::main_menu::settings_callback const &);

	fcppt::signal::auto_connection
	register_start_callback(
		fruitapp::gui::dialogs::main_menu::start_callback const &);

	fcppt::signal::auto_connection
	register_highscore_callback(
		fruitapp::gui::dialogs::main_menu::highscore_callback const &);

	~main_menu();
private:
	fruitlib::scenic::adaptors::gui_system gui_node_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	fruitapp::gui::ce::button settings_button_;
	fruitapp::gui::ce::button highscore_button_;
	fruitapp::gui::ce::button quit_button_;
	fruitapp::gui::ce::button start_button_;
};
}
}
}
}

#endif
