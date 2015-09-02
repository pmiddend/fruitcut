#ifndef FRUITAPP_FRUIT_POINT_CLOUD_HPP_INCLUDED
#define FRUITAPP_FRUIT_POINT_CLOUD_HPP_INCLUDED

#include <fruitapp/fruit/triangle.hpp>
#include <fruitlib/def_ctor.hpp>
#include <fruitlib/geometry_traits/vector.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{

typedef
boost::geometry::model::multi_point<
	fruitlib::def_ctor<
		fruitapp::fruit::triangle::vector
	>
>
point_cloud;

}
}

#endif
