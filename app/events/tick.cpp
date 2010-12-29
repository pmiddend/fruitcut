#include "tick.hpp"

fruitcut::app::events::tick::tick(
	sge::time::funit const _delta)
:
	delta_(
		_delta)
{
}

sge::time::funit
fruitcut::app::events::tick::delta() const
{
	return delta_;
}
