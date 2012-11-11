#ifndef FRUITAPP_FRUIT_HULL_INTERSECTION_PAIR_HPP_INCLUDED
#define FRUITAPP_FRUIT_HULL_INTERSECTION_PAIR_HPP_INCLUDED

#include <fruitapp/fruit/hull/ring.hpp>
#include <fcppt/homogenous_pair.hpp>


namespace fruitapp
{
namespace fruit
{
namespace hull
{
typedef
fcppt::homogenous_pair<fruitapp::fruit::hull::ring::value_type>
intersection_pair;
}
}
}

#endif
