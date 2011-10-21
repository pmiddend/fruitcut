#ifndef FRUITSERVER_LISTENER_CALLBACKS_CONNECT_HPP_INCLUDED
#define FRUITSERVER_LISTENER_CALLBACKS_CONNECT_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <tr1/functional>
#include <fruitserver/listener/callbacks/connect_fn.hpp>
#include <fcppt/config/external_end.hpp>


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
