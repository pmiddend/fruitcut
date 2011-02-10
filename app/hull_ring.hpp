#ifndef FRUITCUT_APP_HULL_RING_HPP_INCLUDED
#define FRUITCUT_APP_HULL_RING_HPP_INCLUDED

#include "../geometry_traits/vector.hpp"
#include <sge/renderer/vector2.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <vector>

namespace fruitcut
{
namespace app
{
// renderer::vector2 is arbitrary. The hull could also be output as
// integer vectors.
typedef
boost::geometry::model::ring<sge::renderer::vector2>
hull_ring;
}
}

#endif
