#ifndef FRUITCUT_APP_FRUIT_MAKE_COORDINATE_SYSTEM_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MAKE_COORDINATE_SYSTEM_HPP_INCLUDED

#include "../../math/line/distance_to_point.hpp"
#include "../../math/line/basic.hpp"
#include <fcppt/optional.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/range_compare.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/container/array.hpp>
#include <boost/mpl/identity.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
template<typename Container>
fcppt::optional
<
	typename 
	fcppt::math::matrix::static_
	<
		typename boost::mpl::identity<typename Container::value_type>::type::value_type,
		4,
		4
	>::type
> const
make_coordinate_system(
	Container const &points,
	typename boost::mpl::identity<typename Container::value_type>::type::value_type const epsilon)
{
	typedef typename
	Container::value_type
	vector;

	typedef typename
	vector::value_type
	scalar;

	typedef typename
	Container::size_type
	size_type;

	typedef typename
	Container::const_iterator
	const_iterator;

	typedef typename 
	fcppt::math::matrix::static_<scalar,4,4>::type
	matrix4;

	FCPPT_ASSERT(
		!points.empty());

	const_iterator first_other_point = 
		boost::next(
			points.begin());

	for(; first_other_point != points.end(); ++first_other_point)
		if(
			!fcppt::math::range_compare(
				*points.begin(),
				*first_other_point,
				epsilon))
			break;

	// All the points are equal
	if (first_other_point == points.end())
		return fcppt::optional<matrix4>();

	const_iterator second_other_point = 
		boost::next(
			first_other_point);

	for(; second_other_point != points.end(); ++second_other_point)
		if(
			math::line::distance_to_point(
				*second_other_point,
				math::line::basic<scalar,3>(
					(*points.begin()),
					(*first_other_point) - (*points.begin()))) > epsilon)
			break;

	// All the points lie on one line
	if(second_other_point == points.end())
		return fcppt::optional<matrix4>();

	typedef
	fcppt::container::array<vector,2>
	direction_vectors;

	direction_vectors directions(
		fcppt::assign::make_array<vector>
			((*first_other_point) - (*points.begin()))
			((*second_other_point) - (*points.begin())));

	fcppt::math::vector::orthogonalize(
		directions.begin(),
		directions.end());

	for(
		typename direction_vectors::iterator i = directions.begin(); 
		i != directions.end(); 
		++i)
		(*i) = 
			fcppt::math::vector::normalize(
				*i);
		
	vector const crossed = 
		fcppt::math::vector::cross(
			directions[0],
			directions[1]);

	return 
		matrix4(
			directions[0][0],directions[1][0],crossed[0],(*points.begin())[0],
			directions[0][1],directions[1][1],crossed[1],(*points.begin())[1],
			directions[0][2],directions[1][2],crossed[2],(*points.begin())[2],
			0,0,0,1);
}
}
}
}

#endif
