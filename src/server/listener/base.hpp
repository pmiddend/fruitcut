#ifndef FRUITCUT_SERVER_LISTENER_BASE_HPP_INCLUDED
#define FRUITCUT_SERVER_LISTENER_BASE_HPP_INCLUDED

#include "callbacks/receive_data.hpp"
#include "callbacks/connect.hpp"
#include "callbacks/disconnect.hpp"
#include "../byte_sequence.hpp"
#include <string>

namespace fruitcut
{
namespace server
{
namespace listener
{
class base
{
public:
	virtual void
	run_once() = 0;

	virtual void
	client_connect(
		callbacks::connect const &) = 0;

	virtual void
	client_receive_data(
		callbacks::receive_data const &) = 0;

	virtual void
	client_disconnect(
		callbacks::disconnect const &) = 0;

	virtual void
	send(
		int,
		server::byte_sequence const &) = 0;

	virtual ~base();
protected:
	explicit
	base();
};
}
}
}

#endif
