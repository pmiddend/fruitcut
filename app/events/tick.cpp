#include "tick.hpp"
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/chrono/duration_cast.hpp>

fruitcut::app::events::tick::tick(
	sge::time::duration const &_delta)
:
	delta_(
		_delta)
{
}

sge::time::funit
fruitcut::app::events::tick::delta_ms() const
{
	return 
		static_cast<sge::time::funit>(
			fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
				delta_).count()) /
			static_cast<sge::time::funit>(
				1000);
}

sge::time::duration const &
fruitcut::app::events::tick::delta() const
{
	return delta_;
}
