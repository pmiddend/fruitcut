#ifndef FRUITAPP_GUI_CE_TABLE_VIEW_HPP_INCLUDED
#define FRUITAPP_GUI_CE_TABLE_VIEW_HPP_INCLUDED

#include <fruitapp/gui/ce/table/model_fwd.hpp>
#include <fruitapp/gui/ce/table/row.hpp>
#include <fruitapp/gui/ce/table/row_index.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace CEGUI
{
class Window;
class MultiColumnList;
}

namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace table
{
class view
{
FCPPT_NONCOPYABLE(
	view);
public:
	view(
		CEGUI::Window &,
		fruitapp::gui::ce::table::model &);

	~view();
private:
	CEGUI::MultiColumnList &impl_;
	fruitapp::gui::ce::table::model &model_;
	fcppt::signal::auto_connection row_added_connection_;
	fcppt::signal::auto_connection row_removed_connection_;

	void
	row_added(
		fruitapp::gui::ce::table::row_index::value_type const &,
		fruitapp::gui::ce::table::row const &);

	void
	row_removed(
		fruitapp::gui::ce::table::row_index::value_type const &);
};
}
}
}
}

#endif
