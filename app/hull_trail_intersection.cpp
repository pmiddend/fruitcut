#include "hull_trail_intersection.hpp"
#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/output.hpp>
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
	cursor_lines.resize(
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

	if(hull.empty() || cursor_lines.empty())
		return result_type();
	for(hull_ring::const_iterator i = hull.begin(); i != boost::prior(hull.end()); ++i)
	{
		segment_type hull_segment(
			*i,
			*boost::next(i));
		for (linestring_type::const_iterator j = cursor_lines.begin(); j != boost::prior(cursor_lines.end()); ++j)
		{
			if (fcppt::math::vector::length((*j) - (*boost::next(j))) < static_cast<sge::renderer::scalar>(0.001))
				continue;

			segment_type cursor_segment(
				*j,
				*boost::next(j));

			unsigned before = result.size();
			boost::geometry::intersection(
				hull_segment,
				cursor_segment,
				result);
			if (result.size() != before)
			{
				std::cout << "cursor is: ";
				BOOST_FOREACH(linestring_type::const_reference cursor_point,cursor_lines)
					std::cout << cursor_point << "\n";
				std::cout << "end\n";
			}
		}
	}
	/*
	boost::geometry::intersection(
		hull,
		cursor_lines,
		result);
	*/

	if (result.size() < static_cast<result_points::size_type>(2))
		return result_type();

	return 
		pair_type(
			result[0],
			result[1]);
}
