#ifndef FRUITAPP_HIGHSCORE_PROVIDER_NET_OBJECT_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_NET_OBJECT_HPP_INCLUDED

#include <fruitapp/highscore/provider/connection_base_ptr.hpp>
#include <fruitapp/highscore/provider/identifier.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fruitapp/highscore/provider/net/host.hpp>
#include <fruitapp/highscore/provider/net/port.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/string.hpp>


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
	object(
		net::host const &,
		net::port const &);

	provider::identifier::value_type
	identifier() const
	override;

	provider::connection_base_ptr
	create_connection()
	override;

	~object()
	override;
private:
	net::host::value_type const host_;
	net::port::value_type const port_;
};
}
}
}
}

#endif
