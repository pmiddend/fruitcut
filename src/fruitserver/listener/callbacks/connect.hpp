#ifndef FRUITSERVER_LISTENER_CALLBACKS_CONNECT_HPP_INCLUDED
#define FRUITSERVER_LISTENER_CALLBACKS_CONNECT_HPP_INCLUDED

#include "connect_fn.hpp"
#include <tr1/functional>

namespace fruitserver
{
namespace listener
{
namespace callbacks
{
typedef
std::tr1::function<callbacks::connect_fn>
connect;
}
}
}

#endif
