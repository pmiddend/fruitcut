#ifndef FRUITCUT_MATH_TRIANGLE_PLANE_INTERSECTION_HPP_INCLUDED
#define FRUITCUT_MATH_TRIANGLE_PLANE_INTERSECTION_HPP_INCLUDED

#include "triangle/basic.hpp"
#include <fcppt/math/size_type.hpp>
#include <vector>

namespace fruitcut
{
namespace math
{
template<typename T,fcppt::math::size_type N,typename Data>
class triangle_plane_intersection
{
public:
	typedef
	triangle::basic<T,N,Data>
	triangle_type;

	typedef
	std::vector<triangle_type>
	triangle_sequence;

	typedef
	std::vector<typename triangle_type::vector>
	point_sequence;

	void
	push_back(
		triangle_type const &t)
	{
		triangles_.push_back(t);
	}

	void
	push_back(
		typename point_sequence::value_type const &v)
	{
		points_.push_back(v);
	}

	triangle_sequence const &
	triangles() const
	{
		return triangles_;
	}

	point_sequence const &
	points() const
	{
		return points_;
	}
private:
	triangle_sequence triangles_;
	point_sequence points_;
};
}
}

#endif
