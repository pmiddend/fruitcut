#ifndef FRUITCUT_SERVER_CONTROLLER_HPP_INCLUDED
#define FRUITCUT_SERVER_CONTROLLER_HPP_INCLUDED

#include "listener/base_fwd.hpp"
#include "byte_sequence.hpp"
#include "command_processor_fwd.hpp"
#include <map>
#include <iosfwd>

namespace fruitcut
{
namespace server
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
		server::listener::base &,
		server::command_processor &,
		std::ostream &log_stream);
private:
	typedef
	std::map<int,server::byte_sequence> 
	fd_to_data;

	server::command_processor &command_processor_;
	std::ostream &log_stream_;
	fd_to_data fd_to_data_;

	void
	client_connect(
		int);

	void
	client_receive_data(
		fruitcut::server::listener::base &,
		int,
		server::byte_sequence const &);

	void
	client_disconnect(
		int);
};
}
}

#endif
