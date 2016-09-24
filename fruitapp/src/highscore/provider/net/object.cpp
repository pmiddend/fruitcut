#include <fruitapp/highscore/provider/net/connection.hpp>
#include <fruitapp/highscore/provider/net/object.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/unique_ptr_to_base.hpp>


fruitapp::highscore::provider::net::object::object(
	net::host const &_host,
	net::port const &_port)
:
	host_(
		_host.get()),
	port_(
		_port.get())
{
}

fruitapp::highscore::provider::identifier::value_type
fruitapp::highscore::provider::net::object::identifier() const
{
	return
		host_;
}

fruitapp::highscore::provider::connection_base_ptr
fruitapp::highscore::provider::net::object::create_connection()
{
	return
		fcppt::unique_ptr_to_base<
			fruitapp::highscore::provider::connection_base
		>(
			fcppt::make_unique_ptr<net::connection>(
				host_,
				port_));
}

fruitapp::highscore::provider::net::object::~object()
{
}
