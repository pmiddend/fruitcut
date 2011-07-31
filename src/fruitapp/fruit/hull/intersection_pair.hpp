#ifndef FRUITAPP_FRUIT_HULL_INTERSECTION_PAIR_HPP_INCLUDED
#define FRUITAPP_FRUIT_HULL_INTERSECTION_PAIR_HPP_INCLUDED

#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>
#include "ring.hpp"

namespace fruitapp
{
namespace fruit
{
namespace hull
{
typedef
fcppt::optional
<
	fcppt::homogenous_pair<hull::ring::value_type> 
>
intersection_pair;
}
}
}

#endif
