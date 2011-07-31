#ifndef FRUITAPP_GUI_TABLE_ROW_ADDED_HPP_INCLUDED
#define FRUITAPP_GUI_TABLE_ROW_ADDED_HPP_INCLUDED

#include <fruitapp/gui/table/row_added_fn.hpp>
#include <fcppt/function/object.hpp>

namespace fruitapp
{
namespace gui
{
namespace table
{
typedef
fcppt::function::object<table::row_added_fn>
row_added;
}
}
}

#endif
