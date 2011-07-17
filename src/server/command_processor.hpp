#ifndef FRUITCUT_SERVER_COMMAND_PROCESSOR_HPP_INCLUDED
#define FRUITCUT_SERVER_COMMAND_PROCESSOR_HPP_INCLUDED

#include "command.hpp"
#include "ascii/string.hpp"
#include <iosfwd>
#include <map>

namespace fruitcut
{
namespace server
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
		server::command const &);

	~command_processor();
private:
	typedef
	std::map
	<
		ascii::string,
		ascii::string const (command_processor::*)(server::command const &)
	>
	command_name_to_handler;

	std::ostream &log_stream_;
	std::string const data_dir_;
	command_name_to_handler command_name_to_handler_;

	ascii::string const
	handle_get(
		server::command const &);

	ascii::string const
	handle_put(
		server::command const &);
};
}
}

#endif
