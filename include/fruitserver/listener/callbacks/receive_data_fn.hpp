#ifndef FRUITSERVER_LISTENER_CALLBACKS_RECEIVE_DATA_FN_HPP_INCLUDED
#define FRUITSERVER_LISTENER_CALLBACKS_RECEIVE_DATA_FN_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/byte_sequence.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
{
namespace listener
{
namespace callbacks
{
typedef
void
receive_data_fn(
	int,
	fruitserver::byte_sequence const &);
}
}
}

#endif
