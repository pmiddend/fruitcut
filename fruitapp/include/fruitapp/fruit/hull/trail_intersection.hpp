#ifndef FRUITAPP_FRUIT_HULL_TRAIL_INTERSECTION_HPP_INCLUDED
#define FRUITAPP_FRUIT_HULL_TRAIL_INTERSECTION_HPP_INCLUDED

#include <fruitapp/cursor/sampler.hpp>
#include <fruitapp/fruit/hull/optional_intersection_pair.hpp>
#include <fruitapp/fruit/hull/ring.hpp>
#include <fcppt/homogenous_pair.hpp>
#include <fcppt/optional/object.hpp>


namespace fruitapp
{
namespace fruit
{
namespace hull
{
// Returns either no or two intersection points (three or more
// wouldn't do any good)
fruitapp::fruit::hull::optional_intersection_pair const
trail_intersection(
	fruitapp::fruit::hull::ring const &,
	fruitapp::cursor::sampler::position_buffer const &);
}
}
}

#endif
