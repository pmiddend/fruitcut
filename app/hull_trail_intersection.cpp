#include "hull_trail_intersection.hpp"
#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/foreach.hpp>
#include <iostream>

fcppt::optional
<
	fcppt::homogenous_pair<fruitcut::app::hull_ring::value_type> 
> const
fruitcut::app::hull_trail_intersection(
	hull_ring const &hull,
	cursor_trail::position_buffer const &positions)
{
	typedef
	fcppt::homogenous_pair<fruitcut::app::hull_ring::value_type> 
	pair_type;
	
	typedef
	fcppt::optional
	<
		pair_type
	>
	result_type;

	typedef
	std::vector<hull_ring::value_type>
	result_points;

	typedef
	boost::geometry::model::linestring<hull_ring::value_type>
	linestring_type;

	linestring_type cursor_lines;

	// Can't use the vector constructor with linestring
	cursor_lines.reserve(
		static_cast<linestring_type::size_type>(
			positions.size()));

	BOOST_FOREACH(
		cursor_trail::position_buffer::const_reference cursor_pos,
		positions)
		cursor_lines.push_back(
			fcppt::math::vector::structure_cast<hull_ring::value_type>(
				cursor_pos));

	result_points result;

	typedef
	boost::geometry::model::segment<linestring_type::value_type>
	segment_type;

	boost::geometry::intersection(
		hull,
		cursor_lines,
		result);

	if (result.size() < static_cast<result_points::size_type>(2))
		return result_type();

	return 
		pair_type(
			result[0],
			result[1]);
}
