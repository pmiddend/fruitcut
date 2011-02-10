#include "hull_trail_intersection.hpp"

fcppt::optional
<
	fcppt::homogenous_pair<fruitcut::app::hull_ring::value_type> 
> const
fruitcut::app::hull_trail_intersection(
	hull_ring const &hull,
	cursor_trail::position_buffer const &positions)
{
	fcppt::optional<hull_ring::value_type> first_intersection;
	
	for(hull_ring::const_iterator i = hull.begin(); i != hull.end(); ++i)
	{
		
	}
}
