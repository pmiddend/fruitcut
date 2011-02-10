#ifndef FRUITCUT_APP_HULL_TRAIL_INTERSECTION_HPP_INCLUDED
#define FRUITCUT_APP_HULL_TRAIL_INTERSECTION_HPP_INCLUDED

#include "hull_ring.hpp"
#include "../cursor_trail.hpp"
#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>

namespace fruitcut
{
namespace app
{
// Returns either no or two intersection points (three or more
// wouldn't do any good)
fcppt::optional
<
	fcppt::homogenous_pair<hull_ring::value_type> 
> const
hull_trail_intersection(
	hull_ring const &,
	cursor_trail::position_buffer const &);
}
}

#endif
