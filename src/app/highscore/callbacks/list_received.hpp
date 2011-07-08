#ifndef FRUITCUT_APP_HIGHSCORE_CALLBACKS_LIST_RECEIVED_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_CALLBACKS_LIST_RECEIVED_HPP_INCLUDED

#include "list_received_fn.hpp"
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace app
{
namespace highscore
{
namespace callbacks
{
typedef
fcppt::function::object<list_received_fn>
list_received;
}
}
}
}

#endif
