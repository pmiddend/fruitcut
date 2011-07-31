#include "posix_select.hpp"
#include <fruitserver/logger.hpp>
#include <fruitserver/std_error_string.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <ostream>
#include <cstddef>
#include <algorithm>
#include <iterator>
#include <tr1/array>

fruitserver::listener::posix_select::posix_select(
	short const port,
	int const listen_queue_size,
	std::ostream &_log_stream)
:
	log_stream_(
		_log_stream),
	listening_socket_(
		::socket(
			AF_INET,
			SOCK_STREAM,
			// protocol
			0)),
	master_read_fds_(),
	maximum_fd_(),
	client_connect_(),
	client_receive_data_(),
	client_disconnect_(),
	fd_to_send_()
{
	FD_ZERO(
		&master_read_fds_);

	if(listening_socket_ == -1)
		throw 
			std::runtime_error(
				"listen(): "+
				fruitserver::std_error_string());

	maximum_fd_ = 
		listening_socket_;

	FD_SET(
		listening_socket_,
		&master_read_fds_);

	fruitserver::logger(log_stream_) << "listener: socket creation succeeded";

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
				fruitserver::std_error_string());

	fruitserver::logger(log_stream_) << "listener: bind succeeded";

	if(
		::listen(
			listening_socket_,
			listen_queue_size) == -1)
		throw 
			std::runtime_error(
				"bind(): "+
				fruitserver::std_error_string());

	fruitserver::logger(log_stream_) << "listener: listen succeeded";
}

void
fruitserver::listener::posix_select::run_once()
{
	fd_set read_fds = 
		master_read_fds_;

	fruitserver::logger(log_stream_) << "listener: selecting";

	fd_set write_fds;
	FD_ZERO(
		&write_fds);

	for(
		fd_to_send::const_iterator i = fd_to_send_.begin(); 
		i != fd_to_send_.end(); 
		++i)
	{
		FD_SET(
			i->first,
			&write_fds);
	}

	if(
		::select(
			maximum_fd_+1,
			&read_fds,
			&write_fds,
			0,
			0) == -1)
		std::runtime_error(
			"select(): "+
			fruitserver::std_error_string());

	fruitserver::logger(log_stream_) << "listener: select succeeded";

	for(
		int i = 0;
		i <= maximum_fd_;
		++i)
	{
		if(FD_ISSET(i,&read_fds))
			handle_read(
				i);
		else if (FD_ISSET(i,&write_fds))
			handle_write(
				i);
	}
}

void
fruitserver::listener::posix_select::client_connect(
	callbacks::connect const &_client_connect)
{
	client_connect_ = 
		_client_connect;
}

void
fruitserver::listener::posix_select::client_receive_data(
	callbacks::receive_data const &_client_receive_data)
{
	client_receive_data_ = 
		_client_receive_data;
}

void
fruitserver::listener::posix_select::client_disconnect(
	callbacks::disconnect const &_client_disconnect)
{
	client_disconnect_ =
 		_client_disconnect;
}

void
fruitserver::listener::posix_select::send(
	int const _fd,
	fruitserver::byte_sequence const &_data)
{
	if(_data.empty())
		throw 
			std::runtime_error(
				"We cannot write an empty string");
		
	std::copy(
		_data.begin(),
		_data.end(),
		std::back_inserter(
			fd_to_send_[_fd]));
}

fruitserver::listener::posix_select::~posix_select()
{
}

void
fruitserver::listener::posix_select::handle_read(
	int const fd)
{
	fruitserver::logger(log_stream_) << "listener(" << fd << "): Ah, we've got something on fd " << fd;

	if(fd == listening_socket_)
	{
		fruitserver::logger(log_stream_) << "listener(" << fd << "): It seems to be a new connection, accepting...";

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
				fruitserver::std_error_string());

		fruitserver::logger(log_stream_) 
			<< "listener: The accept worked, we've got a new connection from IP: " 
			<< 
				inet_ntoa(
					remote_addr.sin_addr)
			<< "...";

		FD_SET(
			new_fd,
			&master_read_fds_);
		// Technically dangerous, since maximum_fd controls how long the
		// loop runs in which we're currently in, but it's safe in this
		// situation
		maximum_fd_ = 
			std::max(
				maximum_fd_,
				new_fd);

		if(client_connect_)
			client_connect_(
				new_fd);

		return;
	}

	fruitserver::logger(log_stream_) << "listener(" << fd << "): It's something from a client that's already connected...";

	std::tr1::array<char,1024> buffer;

	ssize_t const bytes_received = 
		::recv(
			fd,
			&buffer[0],
			buffer.size(),
			0);

	if(bytes_received == 0 || bytes_received == -1)
	{
		if(bytes_received == 0)
			fruitserver::logger(log_stream_) 
				<< "listener(" << fd << "): Oh, we've got a disconnect";
		else
			fruitserver::logger(log_stream_) 
				<< "listener(" << fd << "): Crap, an error occured: " << fruitserver::std_error_string();

		if(client_disconnect_)
		{
			client_disconnect_(
				fd);

			fd_to_send_.erase(
				fd);
		}

		if(
			::close(
				fd) == -1)
			std::runtime_error(
				"close(): "+
				fruitserver::std_error_string());

		FD_CLR(
			fd,
			&master_read_fds_);

		return;
	}

	std::string data(
		&buffer[0],
		static_cast<std::string::size_type>(
			bytes_received));

	fruitserver::logger(log_stream_) 
		<< "listener(" << fd << "): We received " << bytes_received << " bytes of data: " << data;

	if(client_receive_data_)
		client_receive_data_(
			fd,
			fruitserver::byte_sequence(
				reinterpret_cast<fruitserver::byte_sequence::value_type const *>(
					&buffer[0]),
				reinterpret_cast<fruitserver::byte_sequence::value_type const *>(
					&buffer[static_cast<std::size_t>(bytes_received)])));
}

void
fruitserver::listener::posix_select::handle_write(
	int const fd)
{
	fruitserver::logger(log_stream_) << "listener(" << fd << "): Ready to write";

	fd_to_send::iterator fd_string_pair = 
		fd_to_send_.find(fd);

	if(fd_string_pair == fd_to_send_.end())
		throw 
			std::runtime_error(
				"Couldn't find the fd to which we want to write something");

	if(fd_string_pair->second.empty())
		throw 
			std::runtime_error(
				"We cannot write an empty string");

	fruitserver::logger(log_stream_) 
		<< "listener(" << fd << "): Trying to send " << fd_string_pair->second.size() << " bytes";

	ssize_t const bytes_sent = 
		::send(
			fd,
			&(fd_string_pair->second)[0],
			static_cast<std::size_t>(
				fd_string_pair->second.size()),
			0);

	if(bytes_sent == -1)
		throw 
			std::runtime_error(
				"send(): "+
				fruitserver::std_error_string());

	fruitserver::logger(log_stream_) 
		<< "listener(" << fd << "): Sent " << bytes_sent << " bytes";

	fd_string_pair->second.erase(
		fd_string_pair->second.begin(),
		fd_string_pair->second.begin() + 
			static_cast<fruitserver::byte_sequence::difference_type>(
				bytes_sent));

	if(fd_string_pair->second.empty())
		fd_to_send_.erase(
			fd);
}
