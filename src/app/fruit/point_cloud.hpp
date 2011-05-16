#ifndef FRUITCUT_APP_FRUIT_POINT_CLOUD_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_POINT_CLOUD_HPP_INCLUDED

#include "triangle.hpp"
#include "../../geometry_traits/vector.hpp"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
boost::geometry::model::multi_point<triangle::vector>
point_cloud;
}
}
}

#endif
