#include "controller.hpp"
#include "listener.hpp"
#include "process_command.hpp"
#include "parse_command.hpp"
#include "logger.hpp"
#include "lexical_cast.hpp"
#include <tr1/functional>
#include <stdexcept>

fruitcut::server::controller::controller(
	server::listener &_listener,
	std::string const &_data_dir,
	std::ostream &_log_stream)
:
	data_dir_(
		_data_dir),
	log_stream_(
		_log_stream),
	fd_to_data_()
{
	_listener.on_client_create(
		std::tr1::bind(
			&controller::client_connect,
			this,
			std::tr1::placeholders::_1,
			std::tr1::placeholders::_2));

	_listener.on_receive_data(
		std::tr1::bind(
			&controller::client_new_data,
			this,
			std::tr1::placeholders::_1,
			std::tr1::placeholders::_2,
			std::tr1::placeholders::_3));

	_listener.on_client_quit(
		std::tr1::bind(
			&controller::client_disconnect,
			this,
			std::tr1::placeholders::_1,
			std::tr1::placeholders::_2));
}

void
fruitcut::server::controller::client_connect(
	fruitcut::server::listener &,
	int const _fd)
{
	fd_to_data_.insert(
		fd_to_data::value_type(
			_fd,
			std::string()));
}

void
fruitcut::server::controller::client_new_data(
	fruitcut::server::listener &_listener,
	int const _fd,
	std::string const &_data)
{
	fd_to_data::iterator found = 
		fd_to_data_.find(
			_fd);

	if(found == fd_to_data_.end())
	{
		throw 
			std::runtime_error(
				"Couldn't find client "+
				fruitcut::server::lexical_cast<std::string>(
					_fd));
	}

	found->second += 
		_data;

	std::string::size_type const newline_pos = 
		found->second.find(
			static_cast<char>(
				// ascii newline
				10));

	if(newline_pos == std::string::npos)
		return;
		
	if(static_cast<std::string::size_type>(_data.length()-1) > newline_pos)
	{
		server::logger(log_stream_) << 
			"Got an invalid package from client " << _fd << "; there was data after the newline:\n\n\""+
			(found->second)+
			"\"";
		return;
	}

	// erase the newline!
	found->second.erase(
		--found->second.end());

	_listener.send(
		_fd,
		fruitcut::server::process_command(
			log_stream_,
			fruitcut::server::parse_command(
				found->second),
			data_dir_));
}

void
fruitcut::server::controller::client_disconnect(
	fruitcut::server::listener &,
	int const _fd)
{
	fd_to_data_.erase(
		_fd);
}
