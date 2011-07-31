#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_MESSAGE_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_MESSAGE_RECEIVED_HPP_INCLUDED

#include "message_received_fn.hpp"
#include <fcppt/function/object.hpp>

namespace fruitapp
{
namespace highscore
{
namespace callbacks
{
typedef
fcppt::function::object<message_received_fn>
message_received;
}
}
}

#endif
