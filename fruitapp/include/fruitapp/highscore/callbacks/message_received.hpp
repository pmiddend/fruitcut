#ifndef FRUITAPP_HIGHSCORE_CALLBACKS_MESSAGE_RECEIVED_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_CALLBACKS_MESSAGE_RECEIVED_HPP_INCLUDED

#include <fruitapp/highscore/callbacks/message_received_fn.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
namespace callbacks
{
typedef
std::function<message_received_fn>
message_received;
}
}
}

#endif
