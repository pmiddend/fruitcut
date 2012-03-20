#ifndef FRUITAPP_HIGHSCORE_PROVIDER_OBJECT_BASE_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_OBJECT_BASE_HPP_INCLUDED

#include <fruitapp/highscore/provider/connection_base_ptr.hpp>
#include <fruitapp/highscore/provider/identifier.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>


namespace fruitapp
{
namespace highscore
{
namespace provider
{
class object_base
{
FCPPT_NONCOPYABLE(
	object_base);
public:
	virtual provider::identifier::value_type const
	identifier() const = 0;

	virtual provider::connection_base_ptr
	create_connection() = 0;

	virtual ~object_base();
protected:
	explicit
	object_base();
};
}
}
}

#endif
