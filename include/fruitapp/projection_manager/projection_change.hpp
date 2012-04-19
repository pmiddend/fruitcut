#ifndef FRUITAPP_PROJECTION_MANAGER_PROJECTION_CHANGE_HPP_INCLUDED
#define FRUITAPP_PROJECTION_MANAGER_PROJECTION_CHANGE_HPP_INCLUDED

#include <fruitlib/perspective_projection_information.hpp>
#include <fruitlib/scenic/events/base.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace projection_manager
{
class projection_change
:
	public fruitlib::scenic::events::base
{
public:
	explicit
	projection_change(
		fruitlib::perspective_projection_information const &);

	fruitlib::perspective_projection_information const &
	perspective_projection_information() const;

	~projection_change();
private:
	fruitlib::perspective_projection_information perspective_projection_information_;
};
}
}

#endif

