#ifndef FRUITCUT_MATH_GIZMO_HPP_INCLUDED
#define FRUITCUT_MATH_GIZMO_HPP_INCLUDED

#include <fcppt/math/vector/vector.hpp>
#include <array>

namespace fruitcut
{
namespace math
{
template<typename Scalar,size_type N>
class gizmo
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

	gizmo &
	position(
		vector const &p)
	{
		position_ = p;
	}

	gizmo &
	right(
		vector const &p)
	{
		array_[0] = p;
	}

	gizmo &
	up(
		vector const &p)
	{
		array_[1] = p;
	}

	gizmo &
	forward(
		vector const &p)
	{
		array_[2] = p;
	}
private:
	vector position_;
	array_type array_;
};
}
}

#endif
