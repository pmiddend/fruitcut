#ifndef FRUITLIB_MATH_TRIANGLE_WEAK_COMPARE_HPP_INCLUDED
#define FRUITLIB_MATH_TRIANGLE_WEAK_COMPARE_HPP_INCLUDED

#include <fruitlib/math/triangle/basic.hpp>
#include <fcppt/math/range_compare.hpp>
#include <fcppt/math/size_type.hpp>
#include <boost/range/numeric.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <algorithm>

namespace fruitlib
{
namespace math
{
namespace triangle
{
template<typename T,fcppt::math::size_type N,typename Data>
bool
weak_compare(
	basic<T,N,Data> const &a,
	basic<T,N,Data> const &b,
	T const &epsilon)
{
	typedef
	basic<T,N,Data>
	value_type;

	// We don't have a nonconst getter for the triangle points, so we
	// copy
	typename value_type::point_array bcopy = b.points();

	// Has to be an iterator so the mutating algorithm below works
	typename value_type::point_array::iterator
		first_in_second =
			std::find_if(
				bcopy.begin(),
				bcopy.end(),
				boost::phoenix::bind(
					&fcppt::math::range_compare
					<
						typename value_type::vector,
						typename value_type::vector,
						T
					>,
					boost::phoenix::arg_names::arg1,
					a.points().front(),
					epsilon));

	if (first_in_second == bcopy.end())
		return false;

	std::rotate(
		bcopy.begin(),
		first_in_second,
		bcopy.end());

	return
		boost::inner_product(
			a.points(),
			bcopy,
			true,
			std::logical_and<bool>(),
			boost::phoenix::bind(
				&fcppt::math::range_compare
				<
					typename value_type::vector,
					typename value_type::vector,
					T
				>,
				boost::phoenix::arg_names::arg1,
				boost::phoenix::arg_names::arg2,
				epsilon));
}
}
}
}

#endif
