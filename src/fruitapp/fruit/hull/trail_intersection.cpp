#include <fruitapp/fruit/hull/trail_intersection.hpp>
#include <fruitapp/fruit/hull/intersection_pair.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <boost/geometry/geometry.hpp>
#include <iostream>

fruitapp::fruit::hull::intersection_pair const
fruitapp::fruit::hull::trail_intersection(
	ring const &hull,
	fruitapp::cursor_trail::position_buffer const &positions)
{
	typedef
	std::vector<ring::value_type>
	result_points;

	typedef
	boost::geometry::model::linestring<ring::value_type>
	linestring_type;

	linestring_type cursor_lines;

	// Can't use the vector constructor with linestring
	cursor_lines.reserve(
		static_cast<linestring_type::size_type>(
			positions.size()));

	for(
		fruitapp::cursor_trail::position_buffer::const_iterator cursor_pos =
			positions.begin();
		cursor_pos != positions.end();
		++cursor_pos)
		cursor_lines.push_back(
			fcppt::math::vector::structure_cast<ring::value_type>(
				*cursor_pos));

	result_points result;

	typedef
	boost::geometry::model::segment<linestring_type::value_type>
	segment_type;

	boost::geometry::intersection(
		hull,
		cursor_lines,
		result);

	if (result.size() < static_cast<result_points::size_type>(2))
		return intersection_pair();

	return
		intersection_pair::value_type(
			result[0],
			result[1]);
}
