#ifndef FRUITAPP_GUI_CE_TABLE_ROW_REMOVED_HPP_INCLUDED
#define FRUITAPP_GUI_CE_TABLE_ROW_REMOVED_HPP_INCLUDED

#include <fruitapp/gui/ce/table/row_removed_fn.hpp>
#include <fcppt/function/object.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace table
{
typedef
fcppt::function::object<fruitapp::gui::ce::table::row_removed_fn>
row_removed;
}
}
}
}

#endif
