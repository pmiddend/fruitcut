#include <fruitapp/fruit/hull/intersection_pair.hpp>
#include <fruitapp/fruit/hull/trail_intersection.hpp>
#include <fcppt/homogenous_pair.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/optional_impl.hpp>
#include <fcppt/cast/int_to_float_fun.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::hull::optional_intersection_pair const
fruitapp::fruit::hull::trail_intersection(
	fruitapp::fruit::hull::ring const &hull,
	fruitapp::cursor::sampler::position_buffer const &positions)
{
	typedef
	std::vector<fruitapp::fruit::hull::ring::value_type>
	result_points;

	typedef
	boost::geometry::model::linestring<fruitapp::fruit::hull::ring::value_type>
	linestring_type;

	linestring_type cursor_lines;

	// Can't use the vector constructor with linestring
	cursor_lines.reserve(
		static_cast<linestring_type::size_type>(
			positions.size()));

	for(
		fruitapp::cursor::sampler::position_buffer::const_iterator cursor_pos =
			positions.begin();
		cursor_pos != positions.end();
		++cursor_pos)
		cursor_lines.push_back(
			fcppt::math::vector::structure_cast<ring::value_type, fcppt::cast::int_to_float_fun>(
				*cursor_pos));

	result_points result;

	boost::geometry::intersection(
		hull,
		cursor_lines,
		result);

	if (result.size() < static_cast<result_points::size_type>(2))
		return
			fruitapp::fruit::hull::optional_intersection_pair();

	if(fcppt::math::vector::length(result[0] - result[1]) < 0.1f)
		return
			fruitapp::fruit::hull::optional_intersection_pair();

	return
		fruitapp::fruit::hull::optional_intersection_pair(
			fruitapp::fruit::hull::intersection_pair(
				result[0],
				result[1]));
}
