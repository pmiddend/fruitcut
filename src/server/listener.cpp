#include "listener.hpp"
#include "std_error_string.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <tr1/array>

fruitcut::server::listener::listener(
	short const port,
	int const listen_queue_size,
	client_create_callback const &_on_client_create,
	client_data_callback const &_on_receive_data,
	client_quit_callback const &_on_client_quit)
:
	listening_socket_(
		::socket(
			AF_INET,
			SOCK_STREAM,
			// protocol
			0)),
	master_fds_(),
	maximum_fd_(),
	on_client_create_(
		_on_client_create),
	on_receive_data_(
		_on_receive_data),
	on_client_quit_(
		_on_client_quit)
{
	FD_ZERO(
		&master_fds_);

	if(listening_socket_ == -1)
		throw 
			std::runtime_error(
				"listen(): "+
				server::std_error_string());

	maximum_fd_ = 
		listening_socket_;

	FD_SET(
		listening_socket_,
		&master_fds_);

	std::cout << "listener: socket creation succeeded\n";

	struct ::sockaddr_in my_address;
	my_address.sin_family = 
		AF_INET;
	my_address.sin_port =
		htons(
			port);
	my_address.sin_addr.s_addr = INADDR_ANY;
	std::memset(
		&my_address.sin_zero,
		0,
		8);

	if(
		::bind(
			listening_socket_,
			reinterpret_cast<struct sockaddr *>(
				&my_address),
			sizeof(
				struct sockaddr)) == -1)
		throw 
			std::runtime_error(
				"bind(): "+
				server::std_error_string());

	std::cout << "listener: bind succeeded\n";

	if(
		::listen(
			listening_socket_,
			listen_queue_size) == -1)
		throw 
			std::runtime_error(
				"bind(): "+
				server::std_error_string());

	std::cout << "listener: listen succeeded\n";
}

void
fruitcut::server::listener::run_once()
{
	fd_set read_fds = 
		master_fds_;

	std::cout << "listener: selecting\n";

	if(
		::select(
			maximum_fd_+1,
			&read_fds,
			0,
			0,
			0) == -1)
		std::runtime_error(
			"select(): "+
			server::std_error_string());

	std::cout << "listener: select succeeded\n";

	for(
		int i = 0;
		i <= maximum_fd_;
		++i)
	{
		if(!FD_ISSET(i,&read_fds))
			continue;

		std::cout << "listener(" << i << "): Ah, we've got something on fd " << i << "\n";

		if(i == listening_socket_)
		{
			std::cout << "listener(" << i << "): It seems to be a new connection, accepting...\n";

			struct sockaddr_in remote_addr;
			socklen_t addr_len = 
				sizeof(
					remote_addr);
			int const new_fd = 
				::accept(
					listening_socket_,
					reinterpret_cast<struct sockaddr *>(
						&remote_addr),
					&addr_len);

			if(new_fd == -1)
				std::runtime_error(
					"accept(): "+
					server::std_error_string());

			std::cout 
				<< "listener: The accept worked, we've got a new connection from IP: " 
				<< 
					inet_ntoa(
						remote_addr.sin_addr)
				<< "...\n";

			FD_SET(
				new_fd,
				&master_fds_);
			// Technically dangerous, since maximum_fd controls how long the
			// loop runs in which we're currently in, but it's safe in this
			// situation
			maximum_fd_ = 
				std::max(
					maximum_fd_,
					new_fd);

			if(on_client_create_)
				on_client_create_(
					std::tr1::ref(
						*this),
					new_fd);

			continue;
		}

		std::cout << "listener(" << i << "): It's something from a client that's already connected...\n";

		std::tr1::array<char,1024> buffer;

		ssize_t const bytes_received = 
			::recv(
				i,
				&buffer[0],
				buffer.size(),
				0);

		if(bytes_received == 0 || bytes_received == -1)
		{
			if(bytes_received == 0)
				std::cout << "listener(" << i << "): Oh, we've got a disconnect from " << i << "\n";
			else
				std::cout << "listener(" << i << "): Crap, an error occured: " << server::std_error_string() << "\n";

			if(on_client_quit_)
				on_client_quit_(
					std::tr1::ref(
						*this),
					i);

			if(
				::close(
					i) == -1)
				std::runtime_error(
					"close(): "+
					server::std_error_string());

			FD_CLR(
				i,
				&master_fds_);

			continue;
		}

		std::string data(
			&buffer[0],
			static_cast<std::string::size_type>(
				bytes_received));

		std::cout << "listener(" << i << "): We received " << bytes_received << " bytes of data: " << data << "\n";

		if(on_receive_data_)
			on_receive_data_(
				std::tr1::ref(
					*this),
				i,
				data);
	}
}

fruitcut::server::listener::~listener()
{
}
