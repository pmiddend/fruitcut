#ifndef FRUITSERVER_LISTENER_POSIX_SELECT_HPP_INCLUDED
#define FRUITSERVER_LISTENER_POSIX_SELECT_HPP_INCLUDED

#include <fruitserver/listener/base.hpp>
#include <fruitserver/byte_sequence.hpp>
#include <fruitserver/listener/callbacks/receive_data.hpp>
#include <fruitserver/listener/callbacks/connect.hpp>
#include <fruitserver/listener/callbacks/disconnect.hpp>
#include <string>
#include <map>
#include <iosfwd>
#include <sys/select.h>

namespace fruitserver
{
namespace listener
{
class posix_select
:
	public listener::base
{
public:
	explicit
	posix_select(
		short port,
		int listen_queue_size,
		std::ostream &);

	void
	run_once();

	void
	client_connect(
		callbacks::connect const &);

	void
	client_receive_data(
		callbacks::receive_data const &);

	void
	client_disconnect(
		callbacks::disconnect const &);

	void
	send(
		int,
		fruitserver::byte_sequence const &);

	~posix_select();
private:
	typedef
	std::map<int,fruitserver::byte_sequence>
	fd_to_send;

	std::ostream &log_stream_;
	int listening_socket_;
	fd_set master_read_fds_;
	int maximum_fd_;
	callbacks::connect client_connect_;
	callbacks::receive_data client_receive_data_;
	callbacks::disconnect client_disconnect_;
	fd_to_send fd_to_send_;

	void
	handle_read(
		int);

	void
	handle_write(
		int);
};
}
}

#endif
