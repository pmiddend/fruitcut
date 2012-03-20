#ifndef FRUITSERVER_LISTENER_CALLBACKS_RECEIVE_DATA_HPP_INCLUDED
#define FRUITSERVER_LISTENER_CALLBACKS_RECEIVE_DATA_HPP_INCLUDED

#include <tr1/functional>
#include <fruitserver/listener/callbacks/receive_data_fn.hpp>


namespace fruitserver
{
namespace listener
{
namespace callbacks
{
typedef
std::tr1::function<callbacks::receive_data_fn>
receive_data;
}
}
}

#endif
