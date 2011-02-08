// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Copyright Barend Gehrels 2010, Geodan, Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_MULTI_ALGORITHMS_EQUALS_HPP
#define BOOST_GEOMETRY_MULTI_ALGORITHMS_EQUALS_HPP


#include <boost/geometry/multi/core/is_multi.hpp>
#include <boost/geometry/multi/core/tags.hpp>
#include <boost/geometry/multi/core/geometry_id.hpp>

#include <boost/geometry/algorithms/equals.hpp>


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template <typename MultiPolygon1, typename MultiPolygon2>
struct equals
    <
        multi_polygon_tag, multi_polygon_tag,
        true, true,
        MultiPolygon1, MultiPolygon2,
        2
    >
    : detail::equals::equals_by_collection
        <
            MultiPolygon1, MultiPolygon2,
            detail::equals::area_check
        >
{};


template <typename Polygon, typename MultiPolygon>
struct equals
    <
        polygon_tag, multi_polygon_tag,
        false, true,
        Polygon, MultiPolygon,
        2
    >
    : detail::equals::equals_by_collection
        <
            Polygon, MultiPolygon,
            detail::equals::area_check
        >
{};


} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH


}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_MULTI_ALGORITHMS_EQUALS_HPP

