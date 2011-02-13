#ifndef FRUITCUT_APP_FRUIT_HULL_TRAIL_INTERSECTION_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_HULL_TRAIL_INTERSECTION_HPP_INCLUDED

#include "ring.hpp"
#include "intersection_pair.hpp"
#include "../../../cursor_trail.hpp"
#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace hull
{
// Returns either no or two intersection points (three or more
// wouldn't do any good)
intersection_pair const
trail_intersection(
	ring const &,
	cursor_trail::position_buffer const &);
}
}
}
}

#endif
