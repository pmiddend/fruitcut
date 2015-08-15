#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_MESSAGE_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_MESSAGE_RECEIVED_HPP_INCLUDED

#include <fruitapp/highscore/callbacks/message_received_fn.hpp>
#include <fcppt/function_impl.hpp>


namespace fruitapp
{
namespace highscore
{
namespace callbacks
{
typedef
fcppt::function<message_received_fn>
message_received;
}
}
}

#endif
