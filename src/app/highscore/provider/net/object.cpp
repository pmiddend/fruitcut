#include "object.hpp"
#include "connection.hpp"
#include <fcppt/make_unique_ptr.hpp>

fruitcut::app::highscore::provider::net::object::object(
	net::host const &_host,
	net::port const &_port)
:
	host_(
		_host.get()),
	port_(
		_port.get())
{
}

fruitcut::app::highscore::provider::identifier::value_type const
fruitcut::app::highscore::provider::net::object::identifier() const
{
	return 
		host_;
}

fruitcut::app::highscore::provider::connection_base_ptr
fruitcut::app::highscore::provider::net::object::create_connection()
{
	return
		fruitcut::app::highscore::provider::connection_base_ptr(
			fcppt::make_unique_ptr<net::connection>(
				host_,
				port_));
}

fruitcut::app::highscore::provider::net::object::~object()
{
}
