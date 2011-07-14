#ifndef FRUITCUT_SERVER_LISTENER_HPP_INCLUDED
#define FRUITCUT_SERVER_LISTENER_HPP_INCLUDED

#include <sys/select.h>
#include <tr1/functional>
#include <string>
#include <iosfwd>
#include <map>

namespace fruitcut
{
namespace server
{
/**
	This class is the "network" layer, doing the following:

	- Listen on a port
	- Accept connections (and call the user)
	- Call the user when new data arrives
	- Call the user when a client disconnects
	- Let the user send data to a client (queueing it up until the local port is ready to send)
 */
class listener
{
public:
	typedef
	std::tr1::function<void(server::listener &,int)>
	client_create_callback;

	typedef
	std::tr1::function<void(server::listener &,int,std::string)>
	client_data_callback;

	typedef
	std::tr1::function<void(server::listener &,int)>
	client_quit_callback;

	explicit
	listener(
		short port,
		int listen_queue_size,
		std::ostream &);

	void
	run_once();

	void
	on_client_create(
		client_create_callback const &);

	void
	on_receive_data(
		client_data_callback const &);

	void
	on_client_quit(
		client_quit_callback const &);

	void
	send(
		int,
		std::string const &);

	~listener();
private:
	typedef
	std::map<int,std::string>
	fd_to_send;

	std::ostream &log_stream_;
	int listening_socket_;
	fd_set master_read_fds_;
	int maximum_fd_;
	client_create_callback on_client_create_;
	client_data_callback on_receive_data_;
	client_quit_callback on_client_quit_;
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
