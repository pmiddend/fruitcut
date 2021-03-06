#ifndef FRUITAPP_FRUIT_MAKE_COORDINATE_SYSTEM_HPP_INCLUDED
#define FRUITAPP_FRUIT_MAKE_COORDINATE_SYSTEM_HPP_INCLUDED

#include <fruitlib/math/line/basic.hpp>
#include <fruitlib/math/line/distance_to_point.hpp>
#include <fruitlib/math/vector/orthogonalize.hpp>
#include <fcppt/literal.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/matrix/row.hpp>
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/vector/comparison.hpp>
#include <fcppt/math/vector/componentwise_equal.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/identity.hpp>
#include <array>
#include <iterator>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
/**
	This function takes a list of points and tries to create a
	"coordinate system" from it. A coordinate system is a set of 4
	points. Three axes (all orthogonal to each other) and the coordinate
	origin.

	The result is a matrix with the columns being these 4 vectors.

	However, there is an optional<> around the matrix. This is for the
	case where all the points are equal, for example.

	Precondition: !points.empty()
 */
template<typename Container>
fcppt::optional::object<
	fcppt::math::matrix::static_<
		typename boost::mpl::identity<typename Container::value_type>::type::value_type,
		4,
		4
	>
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
	Container::const_iterator
	const_iterator;

	typedef
	fcppt::math::matrix::static_<scalar,4,4>
	matrix4;

	typedef
	fcppt::optional::object<matrix4>
	optional_matrix4;

	FCPPT_ASSERT_PRE(
		!points.empty());

	// First step: Find the first point that is not equal to points[0]
	// (so we can form the first axis)
	const_iterator first_other_point =
		std::next(
			points.begin());

	for(; first_other_point != points.end(); ++first_other_point)
		if(
			!fcppt::math::vector::componentwise_equal(
				*points.begin(),
				*first_other_point,
				epsilon))
			break;

	// All the points are equal?
	if (first_other_point == points.end())
		return optional_matrix4();

	// Just to be sure.
	FCPPT_ASSERT_ERROR(
		fcppt::math::vector::length(
			((*first_other_point) - (*points.begin()))) > epsilon);

	// Second step: Find the first point that is not colinear to the
	// first two points (to form the second axis).
	const_iterator second_other_point =
		std::next(
			first_other_point);

	for(; second_other_point != points.end(); ++second_other_point)
		if(
			fruitlib::math::line::distance_to_point(
				*second_other_point,
				fruitlib::math::line::basic<scalar,3>(
					(*points.begin()),
					(*first_other_point) - (*points.begin()))) > epsilon)
			break;

	// All the points lie on one line
	if(second_other_point == points.end())
		return optional_matrix4();

	typedef
	std::array<vector,2>
	direction_vectors;

	// Create the axes and orthonormalize them
	direction_vectors directions{{
		(*first_other_point) - (*points.begin()),
		(*second_other_point) - (*points.begin())}};

	fruitlib::math::vector::orthogonalize(
		directions.begin(),
		directions.end());

	for(
		typename direction_vectors::iterator i = directions.begin();
		i != directions.end();
		++i)
		(*i) =
			fcppt::math::vector::normalize(
				*i);

	// And create the "canonical" third axis via cross
	vector const crossed =
		fcppt::math::vector::cross(
			directions[0],
			directions[1]);

	auto const zero(
		fcppt::literal<
			scalar
		>(
			0
		)
	);

	auto const one(
		fcppt::literal<
			scalar
		>(
			1
		)
	);

	return
		optional_matrix4(
			matrix4(
				fcppt::math::matrix::row(
					directions[0].x(),directions[1].x(),crossed.x(),(*points.begin()).x()
				),
				fcppt::math::matrix::row(
					directions[0].y(),directions[1].y(),crossed.y(),(*points.begin()).y()
				),
				fcppt::math::matrix::row(
					directions[0].z(),directions[1].z(),crossed.z(),(*points.begin()).z()
				),
				fcppt::math::matrix::row(
					zero, zero, zero, one
				)
			)
		);
}
}
}

#endif
