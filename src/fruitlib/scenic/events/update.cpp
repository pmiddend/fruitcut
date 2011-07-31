#include <fruitlib/scenic/events/update.hpp>

fruitlib::scenic::events::update::update(
	scenic::update_duration const &_delta)
:
	events::base(),
	delta_(
		_delta)
{
}

fruitlib::scenic::update_duration const
fruitlib::scenic::events::update::delta() const
{
	return delta_;
}

fruitlib::scenic::events::update::~update()
{
}
