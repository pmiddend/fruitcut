#ifndef FRUITCUT_SERVER_LISTENER_CALLBACKS_DISCONNECT_HPP_INCLUDED
#define FRUITCUT_SERVER_LISTENER_CALLBACKS_DISCONNECT_HPP_INCLUDED

#include "disconnect_fn.hpp"
#include <tr1/functional>

namespace fruitcut
{
namespace server
{
namespace listener
{
namespace callbacks
{
typedef
std::tr1::function<callbacks::disconnect_fn>
disconnect;
}
}
}
}

#endif
