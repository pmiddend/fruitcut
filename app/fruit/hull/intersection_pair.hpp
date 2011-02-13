#ifndef FRUITCUT_APP_FRUIT_HULL_INTERSECTION_PAIR_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_HULL_INTERSECTION_PAIR_HPP_INCLUDED

#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>
#include "ring.hpp"

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace hull
{
typedef
fcppt::optional
<
	fcppt::homogenous_pair<fruitcut::app::fruit::hull::ring::value_type> 
>
intersection_pair;
}
}
}
}

#endif
