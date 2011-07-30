#include "update.hpp"

fruitcut::fruitlib::scenic::events::update::update(
	scenic::update_duration const &_delta)
:
	events::base(),
	delta_(
		_delta)
{
}

fruitcut::fruitlib::scenic::update_duration const
fruitcut::fruitlib::scenic::events::update::delta() const
{
	return delta_;
}

fruitcut::fruitlib::scenic::events::update::~update()
{
}
