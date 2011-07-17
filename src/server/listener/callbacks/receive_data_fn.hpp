#ifndef FRUITCUT_SERVER_LISTENER_CALLBACKS_RECEIVE_DATA_FN_HPP_INCLUDED
#define FRUITCUT_SERVER_LISTENER_CALLBACKS_RECEIVE_DATA_FN_HPP_INCLUDED

#include "../../byte_sequence.hpp"

namespace fruitcut
{
namespace server
{
namespace listener
{
namespace callbacks
{
typedef
void
receive_data_fn(
	int,
	server::byte_sequence const &);
}
}
}
}

#endif
