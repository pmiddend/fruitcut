#ifndef FRUITLIB_MATH_LINE_BASIC_HPP_INCLUDED
#define FRUITLIB_MATH_LINE_BASIC_HPP_INCLUDED

#include <fcppt/assert/pre_message.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/comparison.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/static.hpp>


namespace fruitlib
{
namespace math
{
namespace line
{
template<typename T,fcppt::math::size_type N>
class basic
{
public:
	typedef typename
	fcppt::math::vector::static_<T,N>::type
	vector;

	explicit
	basic(
		vector const &_point,
		vector const &_direction)
	:
		point_(
			_point),
		direction_(
			_direction)
	{
		FCPPT_ASSERT_PRE_MESSAGE(
			direction_ != vector::null(),
			FCPPT_TEXT("Tried to create a line with zero direction vector"));
	}

	vector const &
	point() const
	{
		return point_;
	}

	vector const &
	direction() const
	{
		return direction_;
	}
private:
	vector point_;
	vector direction_;
};
}
}
}

#endif
