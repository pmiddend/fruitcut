#ifndef FRUITCUT_MATH_TRIANGLE_INTERPOLATION_PAIR_HPP_INCLUDED
#define FRUITCUT_MATH_TRIANGLE_INTERPOLATION_PAIR_HPP_INCLUDED

#include "size_type.hpp"
#include "scalar_type.hpp"

namespace fruitcut
{
namespace math
{
namespace triangle
{
template<typename Triangle>
class interpolation_pair
{
public:
	typedef typename
	triangle::scalar_type<Triangle>::type
	scalar;

	explicit
	interpolation_pair(
		triangle::size_type const _index1,
		triangle::size_type const _index2,
		scalar const _value)
	:
		index1_(
			_index1),
		index2_(
			_index2),
		value_(
			_value)
	{
	}

	triangle::size_type
	index1() const
	{
		return index1_;
	}

	triangle::size_type
	index2() const
	{
		return index2_;
	}

	scalar
	value() const
	{
		return value_;
	}
private:
	triangle::size_type index1_,index2_;
	scalar value_;
};
}
}
}

#endif
