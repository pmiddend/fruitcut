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

namespace
{
std::string const
stdstderror()
{
	return 
		::strerror(
			errno);
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

	listener listener_(
		my_options.get<short>(
			"port"),
		my_options.get<int>(
			"listen-queue-size"));

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

