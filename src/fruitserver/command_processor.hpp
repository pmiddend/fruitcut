#ifndef FRUITSERVER_COMMAND_PROCESSOR_HPP_INCLUDED
#define FRUITSERVER_COMMAND_PROCESSOR_HPP_INCLUDED

#include <fruitserver/command.hpp>
#include <fruitserver/ascii/string.hpp>
#include <iosfwd>
#include <map>

namespace fruitserver
{
class command_processor
{
public:
	explicit
	command_processor(
		std::ostream &log_stream,
		std::string const &data_dir);

	ascii::string const
	process(
		fruitserver::command const &);

	~command_processor();
private:
	typedef
	std::map
	<
		ascii::string,
		ascii::string const (command_processor::*)(fruitserver::command const &)
	>
	command_name_to_handler;

	std::ostream &log_stream_;
	std::string const data_dir_;
	command_name_to_handler command_name_to_handler_;

	ascii::string const
	handle_get(
		fruitserver::command const &);

	ascii::string const
	handle_put(
		fruitserver::command const &);
};
}

#endif
