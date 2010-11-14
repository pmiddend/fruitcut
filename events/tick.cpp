#include "tick.hpp"

fruitcut::events::tick::tick(
	scalar const _time_delta)
:
	time_delta_(
		_time_delta)
{
}

fruitcut::scalar
fruitcut::events::tick::time_delta() const
{
	return time_delta_;
}
