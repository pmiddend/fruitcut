#include "hull_trail_intersection.hpp"

// Returns either no or two intersection points (three or more
// wouldn't do any good)
fcppt::optional
<
	fcppt::homogenous_pair<fruitcut::app::hull_ring::value_type> 
> const
fruitcut::app::hull_trail_intersection(
	hull_ring const &,
	cursor_trail::position_buffer const &)
{
}
