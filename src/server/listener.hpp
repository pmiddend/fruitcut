#ifndef FRUITCUT_SERVER_LISTENER_HPP_INCLUDED
#define FRUITCUT_SERVER_LISTENER_HPP_INCLUDED

#include <sys/select.h>
#include <tr1/functional>
#include <string>

namespace fruitcut
{
namespace server
{
class listener
{
public:
	typedef
	std::tr1::function<void(int)>
	client_create_callback;

	typedef
	std::tr1::function<void(int,std::string)>
	client_data_callback;

	typedef
	std::tr1::function<void(int)>
	client_quit_callback;

	explicit
	listener(
		short port,
		int listen_queue_size,
		client_create_callback const &,
		client_data_callback const &,
		client_quit_callback const &);

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
	int listening_socket_;
	fd_set master_fds_;
	int maximum_fd_;
	client_create_callback on_client_create_;
	client_data_callback on_receive_data_;
	client_quit_callback on_client_quit_;
};
}
}

#endif
