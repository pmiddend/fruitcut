#ifndef FRUITAPP_GUI_CE_DIALOGS_RANKING_HPP_INCLUDED
#define FRUITAPP_GUI_CE_DIALOGS_RANKING_HPP_INCLUDED

#include <fruitapp/gui/ce/button.hpp>
#include <fruitapp/gui/ce/post_model.hpp>
#include <fruitapp/gui/ce/system_fwd.hpp>
#include <fruitapp/gui/ce/table/view.hpp>
#include <fruitapp/gui/dialogs/ranking.hpp>
#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/score.hpp>
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
class ranking
:
	public fruitapp::gui::dialogs::ranking
{
FCPPT_NONCOPYABLE(
	ranking);
public:
	ranking(
		fruitapp::gui::ce::system &,
		fruitapp::highscore::provider_sequence &);

	fcppt::signal::auto_connection
	register_highscore_callback(
		fruitapp::gui::dialogs::ranking::highscore_callback const &);

	fcppt::signal::auto_connection
	register_main_menu_callback(
		fruitapp::gui::dialogs::ranking::main_menu_callback const &);

	void
	append_log(
		fcppt::string const &);

	void
	post(
		fruitapp::highscore::name const &,
		fruitapp::highscore::score const &);

	void
	update();

	~ranking();
private:
	fruitlib::scenic::adaptors::ce::gui_system gui_node_;
	sge::cegui::default_keyboard gui_keyboard_;
	sge::cegui::default_cursor gui_cursor_;
	sge::cegui::toolbox::scoped_layout layout_;
	sge::cegui::toolbox::scoped_gui_sheet gui_sheet_;
	fruitapp::gui::ce::button highscore_button_;
	fruitapp::gui::ce::button main_menu_button_;
	fruitapp::highscore::provider_sequence &providers_;
	fruitapp::gui::ce::post_model post_model_;
	fruitapp::gui::ce::table::view table_view_;
	fcppt::signal::scoped_connection message_received_connection_;
	fcppt::signal::scoped_connection error_received_connection_;
};
}
}
}
}

#endif
