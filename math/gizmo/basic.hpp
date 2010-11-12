#ifndef FRUITCUT_MATH_GIZMO_BASIC_HPP_INCLUDED
#define FRUITCUT_MATH_GIZMO_BASIC_HPP_INCLUDED

#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <array>

namespace fruitcut
{
namespace math
{
namespace gizmo
{
template<typename Scalar,fcppt::math::size_type N>
class basic
{
public:
	typedef 
	Scalar 
	scalar;

	typedef typename
	fcppt::math::vector::static_<scalar,N>::type
	vector;

	typedef
	std::array<vector,3>
	array_type;

	vector const &
	position() const
	{
		return position_;
	}

	vector const &
	right() const
	{
		return array_[0];
	}

	vector const &
	up() const
	{
		return array_[1];
	}

	vector const &
	forward() const
	{
		return array_[2];
	}

	array_type const &
	array() const
	{
		return array_;
	}

	array_type &
	array()
	{
		return array_;
	}

	basic &
	position(
		vector const &p)
	{
		position_ = p;
		return *this;
	}

	basic &
	right(
		vector const &p)
	{
		array_[0] = p;
		return *this;
	}

	basic &
	up(
		vector const &p)
	{
		array_[1] = p;
		return *this;
	}

	basic &
	forward(
		vector const &p)
	{
		array_[2] = p;
		return *this;
	}
private:
	vector position_;
	array_type array_;
};
}
}
}

#endif
