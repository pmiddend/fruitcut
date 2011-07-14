#ifndef FRUITCUT_SERVER_CONTROLLER_HPP_INCLUDED
#define FRUITCUT_SERVER_CONTROLLER_HPP_INCLUDED

#include "listener_fwd.hpp"
#include <string>
#include <map>
#include <iosfwd>

namespace fruitcut
{
namespace server
{
class controller
{
public:
	explicit
	controller(
		server::listener &,
		std::string const &data_dir,
		std::ostream &log_stream);
private:
	typedef
	std::map<int,std::string> 
	fd_to_data;

	std::string const data_dir_;
	std::ostream &log_stream_;
	fd_to_data fd_to_data_;

	void
	client_connect(
		fruitcut::server::listener &,
		int);

	void
	client_new_data(
		fruitcut::server::listener &,
		int,
		std::string const &);

	void
	client_disconnect(
		fruitcut::server::listener &,
		int);
};
}
}

#endif
