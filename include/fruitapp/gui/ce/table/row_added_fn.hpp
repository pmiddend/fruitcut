#ifndef FRUITAPP_GUI_CE_TABLE_ROW_ADDED_FN_HPP_INCLUDED
#define FRUITAPP_GUI_CE_TABLE_ROW_ADDED_FN_HPP_INCLUDED

#include <fruitapp/gui/ce/table/row.hpp>
#include <fruitapp/gui/ce/table/row_index.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace table
{
typedef
void
row_added_fn(
	fruitapp::gui::ce::table::row_index::value_type const &,
	fruitapp::gui::ce::table::row const &);
}
}
}
}

#endif
