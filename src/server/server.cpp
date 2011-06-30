#include "listener.hpp"
#include "program_options/help_was_needed.hpp"
#include "program_options/object.hpp"
#include "program_options/make_command_line_parameters.hpp"
#include "program_options/option.hpp"
#include "program_options/option_sequence.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <stdexcept>
#include <sstream>
#include <tr1/functional>
#include <map>

namespace
{
typedef
std::map<int,std::string> 
fd_to_string;

void
add_to_map(
	fd_to_string &_fd_to_string,
	int const _fd)
{
	_fd_to_string.insert(
		fd_to_string::value_type(
			_fd,
			std::string()));
}

void
process_new_data(
	fd_to_string &_fd_to_string,
	int const _fd,
	std::string const &_data)
{
	fd_to_string::iterator found = 
		_fd_to_string.find(
			_fd);

	if(found == _fd_to_string.end())
	{
		std::stringstream ss;
		ss << "Couldn't find client " << _fd << " in the client list!\n";
		throw 
			std::runtime_error(
				ss.str());
	}

	found->second += 
		_data;

	std::string::size_type const newline_pos = 
		found->second.find('\n');

	if(newline_pos == std::string::npos)
		return;
		
	if(static_cast<std::string::size_type>(_data.length()-1) > newline_pos)
		throw 
			std::runtime_error(
				"Got an invalid package; there was data after the newline:\n\n\""+(found->second)+"\"");
}

void
remove_from_map(
	fd_to_string &_fd_to_string,
	int const _fd)
{
	_fd_to_string.erase(
		_fd);
}
}

int 
main(
	int argc,
	char *argv[]) 
try
{
	using namespace fruitcut::server;

	program_options::object my_options(
		program_options::option_sequence()
			(program_options::option<short>("port",1337))
			(program_options::option<std::string>("data-dir",""))
			(program_options::option<int>("listen-queue-size",10)),
		program_options::make_command_line_parameters(
			argc,
			argv));

	fd_to_string fd_to_string_;

	listener listener_(
		my_options.get<short>(
			"port"),
		my_options.get<int>(
			"listen-queue-size"),
		std::tr1::bind(
			&add_to_map,
			std::tr1::ref(
				fd_to_string_),
			std::tr1::placeholders::_1),
		std::tr1::bind(
			&process_new_data,
			std::tr1::ref(
				fd_to_string_),
			std::tr1::placeholders::_1,
			std::tr1::placeholders::_2),
		std::tr1::bind(
			&remove_from_map,
			std::tr1::ref(
				fd_to_string_),
			std::tr1::placeholders::_1));

	while(true)
		listener_.run_once();
}
catch(
	fruitcut::server::program_options::help_was_needed const &e)
{
	std::cerr << e.help_string() << "\n";
	return EXIT_FAILURE;
}
catch(
	std::exception const &e)
{
	std::cerr << "An error occured:\n\n" << e.what() << "\n";
	return EXIT_FAILURE;
}

