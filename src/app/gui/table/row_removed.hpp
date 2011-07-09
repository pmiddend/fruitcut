#ifndef FRUITCUT_APP_GUI_TABLE_ROW_REMOVED_HPP_INCLUDED
#define FRUITCUT_APP_GUI_TABLE_ROW_REMOVED_HPP_INCLUDED

#include "row_removed_fn.hpp"
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace app
{
namespace gui
{
namespace table
{
typedef
fcppt::function::object<table::row_removed_fn>
row_removed;
}
}
}
}

#endif
