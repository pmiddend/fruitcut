#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_LIST_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_LIST_RECEIVED_HPP_INCLUDED

#include <fruitapp/highscore/callbacks/list_received_fn.hpp>
#include <fcppt/function/object.hpp>

namespace fruitapp
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

#endif
