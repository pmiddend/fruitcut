#ifndef FRUITAPP_GUI_CE_TABLE_ROW_ADDED_HPP_INCLUDED
#define FRUITAPP_GUI_CE_TABLE_ROW_ADDED_HPP_INCLUDED

#include <fruitapp/gui/ce/table/row_added_fn.hpp>
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
std::function<fruitapp::gui::ce::table::row_added_fn>
row_added;
}
}
}
}

#endif
