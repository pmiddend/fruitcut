#include "tick.hpp"

fruitcut::sandbox::cut_test::events::tick::tick(
	sge::renderer::scalar const _time_delta)
:
	time_delta_(
		_time_delta)
{
}

sge::renderer::scalar
fruitcut::sandbox::cut_test::events::tick::time_delta() const
{
	return time_delta_;
}
