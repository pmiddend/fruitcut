#include "controller.hpp"
#include "listener/base.hpp"
#include "command_processor.hpp"
#include "parse_command.hpp"
#include "logger.hpp"
#include "lexical_cast.hpp"
#include "format_output_size.hpp"
#include "ascii/string.hpp"
#include "ascii/from_native_char.hpp"
#include "ascii/to_native.hpp"
#include "ascii/from_native.hpp"
#include "ascii/to_byte_sequence.hpp"
#include "ascii/from_byte_sequence.hpp"
#include <tr1/functional>
#include <stdexcept>
#include <iterator>
#include <algorithm>

fruitserver::controller::controller(
	fruitserver::listener::base &_listener,
	fruitserver::command_processor &_command_processor,
	std::ostream &_log_stream)
:
	command_processor_(
		_command_processor),
	log_stream_(
		_log_stream),
	fd_to_data_()
{
	_listener.client_connect(
		std::tr1::bind(
			&controller::client_connect,
			this,
			std::tr1::placeholders::_1));

	_listener.client_receive_data(
		std::tr1::bind(
			&controller::client_receive_data,
			this,
			std::tr1::ref(
				_listener),
			std::tr1::placeholders::_1,
			std::tr1::placeholders::_2));

	_listener.client_disconnect(
		std::tr1::bind(
			&controller::client_disconnect,
			this,
			std::tr1::placeholders::_1));
}

void
fruitserver::controller::client_connect(
	int const _fd)
{
	fd_to_data_.insert(
		fd_to_data::value_type(
			_fd,
			fruitserver::byte_sequence()));
}

void
fruitserver::controller::client_receive_data(
	fruitserver::listener::base &_listener,
	int const _fd,
	fruitserver::byte_sequence const &_data)
{
	fd_to_data::iterator found = 
		fd_to_data_.find(
			_fd);

	if(found == fd_to_data_.end())
	{
		throw 
			std::runtime_error(
				"Couldn't find client "+
				fruitserver::lexical_cast<std::string>(
					_fd));
	}

	std::copy(
		_data.begin(),
		_data.end(),
		std::back_inserter(
			found->second));

	ascii::string converted = 
		ascii::from_byte_sequence(
			found->second);

	ascii::string::size_type const newline_pos = 
		converted.find(
			ascii::from_native_char(
				'\n'));

	if(newline_pos == ascii::string::npos)
		return;
		
	if(static_cast<ascii::string::size_type>(converted.length()-1) > newline_pos)
	{
		fruitserver::logger(log_stream_) << 
			"Got an invalid package from client " << _fd << "; there was data after the newline:\n\n\""+
			ascii::to_native(
				converted)+
			"\"";
		return;
	}

	// erase the newline!
	converted.erase(
		--converted.end());

	ascii::string output = 
		command_processor_.process(
			fruitserver::parse_command(
				converted));

	// Add size
	output = 
		ascii::from_native(
			fruitserver::format_output_size(
				output.size()))+
		output;

	fruitserver::logger(log_stream_) << "Sending: \"" << ascii::to_native(output) << "\"...";

	_listener.send(
		_fd,
		ascii::to_byte_sequence(
			output));
}

void
fruitserver::controller::client_disconnect(
	int const _fd)
{
	fd_to_data_.erase(
		_fd);
}
