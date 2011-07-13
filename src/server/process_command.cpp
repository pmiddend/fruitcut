#include "process_command.hpp"
#include "current_datetime.hpp"
#include "ascii_to_native.hpp"
#include "ascii_to_native_char.hpp"
#include "logger.hpp"

namespace
{
std::string
make_error_response(
	std::string const &s)
{
	return "{ \"error\" : \""+s+"\" }";
}
}

std::string
fruitcut::server::process_command(
	std::ostream &log_stream,
	server::command const &c,
	std::string const &data_dir)
{
	if(
		c.empty())
	{
		server::logger(log_stream) 
			<< std::string(": Error: got an empty command");
		return 
			make_error_response("Got an empty command");
	}

	if(c[0].size() != 1u)
	{
		server::logger(log_stream) 
			<< ": Error: got an invalid command: " 
			<< server::ascii_to_native(c[0]);
		return
			make_error_response(
				"Command \""+c[0]+"\" invalid");
	}

	if(c.size() != 2u)
	{
		server::logger(log_stream) 
			<< ": Error: the command: " 
			<< server::ascii_to_native(c[0])
			<< " doesn't get two parameters";
		return
			make_error_response(
				"Command \""+c[0]+"\" got an invalid number of parameters");
	}

	char const command_name = 
		server::ascii_to_native_char(
			c[0][0]);

	/*
	if(command_name == 'G')
	{
		return 
			server::file_to_string(
				data_dir+"/"+);
	}
	*/
	return "";
}
