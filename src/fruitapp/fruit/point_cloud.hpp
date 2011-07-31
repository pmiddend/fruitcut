#ifndef FRUITAPP_FRUIT_POINT_CLOUD_HPP_INCLUDED
#define FRUITAPP_FRUIT_POINT_CLOUD_HPP_INCLUDED

#include "triangle.hpp"
#include "../../fruitlib/geometry_traits/vector.hpp"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace fruitapp
{
namespace fruit
{
typedef
boost::geometry::model::multi_point<triangle::vector>
point_cloud;
}
}

#endif
