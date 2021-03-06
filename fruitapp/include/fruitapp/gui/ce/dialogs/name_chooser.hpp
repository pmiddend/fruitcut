#ifndef FRUITAPP_GUI_CE_DIALOGS_NAME_CHOOSER_HPP_INCLUDED
#define FRUITAPP_GUI_CE_DIALOGS_NAME_CHOOSER_HPP_INCLUDED

#include <fruitapp/gui/ce/button.hpp>
#include <fruitapp/gui/ce/system_fwd.hpp>
#include <fruitapp/gui/dialogs/name_chooser.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitlib/scenic/adaptors/ce/gui_system.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_focus.hpp>
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
class name_chooser
:
	public fruitapp::gui::dialogs::name_chooser
{
FCPPT_NONCOPYABLE(
	name_chooser);
public:
	name_chooser(
		fruitapp::gui::ce::system &,
		fruitapp::highscore::score const &);

	fcppt::signal::auto_connection
	register_submit_callback(
		fruitapp::gui::dialogs::name_chooser::submit_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_main_menu_callback(
		fruitapp::gui::dialogs::name_chooser::main_menu_callback const &
	)
	override;

	fcppt::signal::auto_connection
	register_restart_callback(
		fruitapp::gui::dialogs::name_chooser::restart_callback const &
	)
	override;

	fcppt::string
	name() const
	override;

	void
	name(
		fcppt::string const &
	)
	override;

	~name_chooser()
	override;
private:
	fruitlib::scenic::adaptors::ce::gui_system gui_node_;
	sge::cegui::default_focus gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	fruitapp::gui::ce::button submit_button_;
	fruitapp::gui::ce::button main_menu_button_;
	fruitapp::gui::ce::button restart_button_;
};
}
}
}
}

#endif
