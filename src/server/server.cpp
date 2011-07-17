#include "program_options/object.hpp"
#include "program_options/option_sequence.hpp"
#include "program_options/option.hpp"
#include "program_options/help_was_needed.hpp"
#include "program_options/make_command_line_parameters.hpp"
#include "listener/posix_select.hpp"
#include "controller.hpp"
#include "command_processor.hpp"
#include <string>
#include <tr1/memory>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <exception>
#include <cstdlib>

int 
main(
	int argc,
	char *argv[]) 
try
{
	fruitcut::server::program_options::object my_options(
		fruitcut::server::program_options::option_sequence()
			(fruitcut::server::program_options::option<short>("port",1337))
			(fruitcut::server::program_options::option<std::string>("data-dir","/tmp"))
			(fruitcut::server::program_options::option<std::string>("log-file","-"))
			(fruitcut::server::program_options::option<int>("listen-queue-size",10)),
		fruitcut::server::program_options::make_command_line_parameters(
			argc,
			argv));

	std::tr1::shared_ptr<std::ofstream> external_log_file;

	std::ostream *log_stream = 
		&std::clog;

	if(my_options.get<std::string>("log-file") != "-")
	{
		external_log_file.reset(
			new std::ofstream(
				my_options.get<std::string>("log-file").c_str()));

		if(!external_log_file->is_open())
			throw 
				std::runtime_error("Couldn't open log file \""+my_options.get<std::string>("log-file")+"\"");

		log_stream  = 
			external_log_file.get();
	}

	std::tr1::shared_ptr<fruitcut::server::listener::posix_select> listener_(
		new fruitcut::server::listener::posix_select(
			my_options.get<short>(
				"port"),
			my_options.get<int>(
				"listen-queue-size"),
			*log_stream));

	fruitcut::server::command_processor processor_(
		*log_stream,
		my_options.get<std::string>("data-dir"));

	fruitcut::server::controller controller_(
		*listener_,
		processor_,
		*log_stream);

	while(true)
		listener_->run_once();
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

