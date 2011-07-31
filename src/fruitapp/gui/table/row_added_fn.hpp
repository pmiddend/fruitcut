#ifndef FRUITAPP_GUI_TABLE_ROW_ADDED_FN_HPP_INCLUDED
#define FRUITAPP_GUI_TABLE_ROW_ADDED_FN_HPP_INCLUDED

#include "row_index.hpp"
#include "row.hpp"

namespace fruitapp
{
namespace gui
{
namespace table
{
typedef
void
row_added_fn(
	table::row_index::value_type const &,
	table::row const &);
}
}
}

#endif
