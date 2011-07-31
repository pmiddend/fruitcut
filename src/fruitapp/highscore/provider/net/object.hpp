#ifndef FRUITAPP_HIGHSCORE_PROVIDER_NET_OBJECT_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_NET_OBJECT_HPP_INCLUDED

#include <fruitapp/highscore/provider/net/port.hpp>
#include <fruitapp/highscore/provider/net/host.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fruitapp/highscore/provider/identifier.hpp>
#include <fruitapp/highscore/provider/connection_base_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace highscore
{
namespace provider
{
namespace net
{
class object
:
	public provider::object_base
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		net::host const &,
		net::port const &);

	provider::identifier::value_type const
	identifier() const;

	provider::connection_base_ptr
	create_connection();

	~object();
private:
	net::host::value_type host_;
	net::port::value_type port_;
};
}
}
}
}

#endif
