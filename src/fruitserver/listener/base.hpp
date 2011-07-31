#ifndef FRUITSERVER_LISTENER_BASE_HPP_INCLUDED
#define FRUITSERVER_LISTENER_BASE_HPP_INCLUDED

#include <fruitserver/listener/callbacks/receive_data.hpp>
#include <fruitserver/listener/callbacks/connect.hpp>
#include <fruitserver/listener/callbacks/disconnect.hpp>
#include <fruitserver/byte_sequence.hpp>
#include <string>

namespace fruitserver
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
		fruitserver::byte_sequence const &) = 0;

	virtual ~base();
protected:
	explicit
	base();
};
}
}

#endif
