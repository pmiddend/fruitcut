#ifndef FRUITCUT_MATH_TRIANGLE_WEAK_COMPARE_HPP_INCLUDED
#define FRUITCUT_MATH_TRIANGLE_WEAK_COMPARE_HPP_INCLUDED

#include "basic.hpp"
#include <fcppt/math/range_compare.hpp>
#include <algorithm>

namespace fruitcut
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
				[&epsilon,&a](
					typename value_type::vector const &v) 
				{ 
					return fcppt::math::range_compare(v,a.points().front(),epsilon); 
				});

	if (first_in_second == bcopy.end())
		return false;

	std::rotate(
		bcopy.begin(),
		first_in_second,
		bcopy.end());

	return 
		std::inner_product(
			a.points().begin(),
			a.points().end(),
			bcopy.begin(),
			true,
			std::logical_and<bool>(),
			[&epsilon](
				typename value_type::vector const &a,
				typename value_type::vector const &b) 
			{
				return 
					fcppt::math::range_compare(a,b,epsilon);
			});
}
}
}
}

#endif
