#ifndef FRUITSERVER_CONTROLLER_HPP_INCLUDED
#define FRUITSERVER_CONTROLLER_HPP_INCLUDED

#include "listener/base_fwd.hpp"
#include "byte_sequence.hpp"
#include "command_processor_fwd.hpp"
#include <map>
#include <iosfwd>

namespace fruitserver
{
/**
	The controller takes a listener and implements the fruitcut protocol
	above it. To manage the content, we have the "process_command" function.
 */
class controller
{
public:
	explicit
	controller(
		fruitserver::listener::base &,
		fruitserver::command_processor &,
		std::ostream &log_stream);
private:
	typedef
	std::map<int,fruitserver::byte_sequence> 
	fd_to_data;

	fruitserver::command_processor &command_processor_;
	std::ostream &log_stream_;
	fd_to_data fd_to_data_;

	void
	client_connect(
		int);

	void
	client_receive_data(
		fruitserver::listener::base &,
		int,
		fruitserver::byte_sequence const &);

	void
	client_disconnect(
		int);
};
}

#endif
