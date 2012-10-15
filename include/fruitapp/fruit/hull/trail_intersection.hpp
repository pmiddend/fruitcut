#ifndef FRUITAPP_FRUIT_HULL_TRAIL_INTERSECTION_HPP_INCLUDED
#define FRUITAPP_FRUIT_HULL_TRAIL_INTERSECTION_HPP_INCLUDED

#include <fruitapp/cursor_trail.hpp>
#include <fruitapp/fruit/hull/intersection_pair.hpp>
#include <fruitapp/fruit/hull/ring.hpp>
#include <fcppt/homogenous_pair.hpp>
#include <fcppt/optional.hpp>


namespace fruitapp
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
	fruitapp::cursor_trail::position_buffer const &);
}
}
}

#endif