#ifndef FRUITCUT_SERVER_LISTENER_CALLBACKS_RECEIVE_DATA_HPP_INCLUDED
#define FRUITCUT_SERVER_LISTENER_CALLBACKS_RECEIVE_DATA_HPP_INCLUDED

#include "receive_data_fn.hpp"
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
std::tr1::function<callbacks::receive_data_fn>
receive_data;
}
}
}
}

#endif
