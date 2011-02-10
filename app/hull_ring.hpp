#ifndef FRUITCUT_APP_HULL_RING_HPP_INCLUDED
#define FRUITCUT_APP_HULL_RING_HPP_INCLUDED

#include "../geometry_traits/vector.hpp"
#include <sge/renderer/vector2.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/geometry/geometry.hpp>
#include <vector>

namespace fruitcut
{
namespace app
{
typedef
// renderer::vector2 is arbitrary. The hull could also be output as
// integer vectors.
// The intersection algorithm doesn't like "ring x linestring"
// intersections, so I use a string here.
//boost::geometry::model::ring<sge::renderer::vector2>
boost::geometry::model::linestring<sge::renderer::vector2>
hull_ring;
}
}

#endif
