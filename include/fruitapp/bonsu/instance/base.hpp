#ifndef FRUITAPP_BONSU_INSTANCE_BASE_HPP_INCLUDED
#define FRUITAPP_BONSU_INSTANCE_BASE_HPP_INCLUDED

#include <fruitapp/bonsu/manager_fwd.hpp>
#include <fruitapp/bonsu/progress_fraction.hpp>
#include <fruitapp/bonsu/texture_identifier.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/intrusive/list.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace bonsu
{
namespace instance
{
class base
:
	public
		boost::intrusive::list_base_hook
		<
			boost::intrusive::link_mode<boost::intrusive::auto_unlink>
		>
{
FCPPT_NONCOPYABLE(
	base);
public:
	virtual ~base();

	virtual void
	update() = 0;

	virtual bonsu::texture_identifier const
	texture() const = 0;

	virtual bool
	dead() const = 0;

	virtual bonsu::progress_fraction
	progress() const = 0;
protected:
	explicit
	base(
		bonsu::manager &);
};
}
}
}

#endif
