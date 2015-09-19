#ifndef FRUITAPP_GUI_CE_DIALOGS_HIGHSCORE_HPP_INCLUDED
#define FRUITAPP_GUI_CE_DIALOGS_HIGHSCORE_HPP_INCLUDED

#include <fruitapp/gui/ce/button.hpp>
#include <fruitapp/gui/ce/combobox.hpp>
#include <fruitapp/gui/ce/get_model.hpp>
#include <fruitapp/gui/ce/system_fwd.hpp>
#include <fruitapp/gui/ce/table/view.hpp>
#include <fruitapp/gui/dialogs/highscore.hpp>
#include <fruitapp/highscore/entry_set.hpp>
#include <fruitapp/highscore/provider_sequence.hpp>
#include <fruitapp/highscore/provider/object_base_fwd.hpp>
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
class highscore
:
	public fruitapp::gui::dialogs::highscore
{
FCPPT_NONCOPYABLE(
	highscore);
public:
	highscore(
		fruitapp::gui::ce::system &,
		fruitapp::highscore::provider_sequence &);

	fcppt::signal::auto_connection
	register_back_callback(
		fruitapp::gui::dialogs::highscore::back_callback const &);

	fcppt::signal::auto_connection
	register_switch_provider_callback(
		fruitapp::gui::dialogs::highscore::switch_provider_callback const &);

	void
	clear_log();

	void
	append_log(
		fcppt::string const &);

	void
	process_list(
		fruitapp::highscore::entry_set const &);

	~highscore();
private:
	typedef
	fcppt::signal::object<fruitapp::gui::dialogs::highscore::switch_provider_function>
	switch_provider_signal;

	switch_provider_signal switch_provider_;
	fruitlib::scenic::adaptors::ce::gui_system gui_node_;
	sge::cegui::default_focus gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	fruitapp::gui::ce::button back_button_;
	fruitapp::highscore::provider_sequence &providers_;
	fruitapp::gui::ce::combobox source_box_;
	fruitapp::gui::ce::get_model table_model_;
	fruitapp::gui::ce::table::view table_view_;
};
}
}
}
}

#endif
