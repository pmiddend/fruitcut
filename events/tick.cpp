#include "tick.hpp"

fruitcut::events::tick::tick(
	sge::renderer::scalar const _time_delta)
:
	time_delta_(
		_time_delta)
{
}

sge::renderer::scalar
fruitcut::events::tick::time_delta() const
{
	return time_delta_;
}
