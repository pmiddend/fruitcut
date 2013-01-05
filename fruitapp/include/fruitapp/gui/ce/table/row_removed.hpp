#ifndef FRUITAPP_GUI_CE_TABLE_ROW_REMOVED_HPP_INCLUDED
#define FRUITAPP_GUI_CE_TABLE_ROW_REMOVED_HPP_INCLUDED

#include <fruitapp/gui/ce/table/row_removed_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace table
{
typedef
std::function<fruitapp::gui::ce::table::row_removed_fn>
row_removed;
}
}
}
}

#endif
