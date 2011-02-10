#include "hull_trail_intersection.hpp"
#include <fcppt/optional.hpp>
#include <fcppt/homogenous_pair.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/geometry/geometry.hpp>

fcppt::optional
<
	fcppt::homogenous_pair<fruitcut::app::hull_ring::value_type> 
> const
fruitcut::app::hull_trail_intersection(
	hull_ring const &hull,
	cursor_trail::position_buffer const &positions)
{
	/*
	typedef
	std::vector<hull_ring::value_type>
	result_vector;

	typedef
	boost::geometry::model::linestring<hull_ring::value_type>
	linestring_type;

	result_vector result;

	boost::geometry::intersection(
		hull,
		);
	*/
}
