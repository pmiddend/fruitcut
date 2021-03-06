#ifndef FRUITAPP_FRUIT_HULL_RING_HPP_INCLUDED
#define FRUITAPP_FRUIT_HULL_RING_HPP_INCLUDED

#include <fruitlib/def_ctor.hpp>
#include <fruitlib/geometry_traits/vector.hpp>
#include <sge/renderer/vector2.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
namespace hull
{
typedef
// renderer::vector2 is arbitrary. The hull could also be output as
// integer vectors.
// The intersection algorithm doesn't like "ring x linestring"
// intersections, so I use a string here.
//boost::geometry::model::ring<sge::renderer::vector2>
boost::geometry::model::linestring<
	fruitlib::def_ctor<
		sge::renderer::vector2
	>
>
ring;

}
}
}

#endif
