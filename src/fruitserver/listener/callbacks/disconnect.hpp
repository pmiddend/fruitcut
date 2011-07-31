#ifndef FRUITSERVER_LISTENER_CALLBACKS_DISCONNECT_HPP_INCLUDED
#define FRUITSERVER_LISTENER_CALLBACKS_DISCONNECT_HPP_INCLUDED

#include "disconnect_fn.hpp"
#include <tr1/functional>

namespace fruitserver
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

#endif
