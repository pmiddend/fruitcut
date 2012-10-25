#ifndef FRUITAPP_GUI_CE_DIALOGS_NAME_CHOOSER_HPP_INCLUDED
#define FRUITAPP_GUI_CE_DIALOGS_NAME_CHOOSER_HPP_INCLUDED

#include <fruitapp/gui/ce/button.hpp>
#include <fruitapp/gui/ce/system_fwd.hpp>
#include <fruitapp/gui/dialogs/name_chooser.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitlib/scenic/adaptors/gui_system.hpp>
#include <sge/cegui/default_cursor.hpp>
#include <sge/cegui/default_keyboard.hpp>
#include <sge/cegui/toolbox/scoped_gui_sheet.hpp>
#include <sge/cegui/toolbox/scoped_layout.hpp>
#include <sge/charconv/system_fwd.hpp>
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
	register_continue_callback(
		fruitapp::gui::dialogs::name_chooser::continue_callback const &);

	fcppt::string const
	name() const;

	~name_chooser();
private:
	sge::charconv::system &charconv_system_;
	fruitlib::scenic::adaptors::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	fruitapp::gui::ce::button continue_button_;
};
}
}
}
}

#endif