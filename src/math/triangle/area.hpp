#ifndef FRUITCUT_MATH_TRIANGLE_AREA_HPP_INCLUDED
#define FRUITCUT_MATH_TRIANGLE_AREA_HPP_INCLUDED

#include "scalar_type.hpp"
#include "vector_type.hpp"
#include "vertex_access.hpp"
#include <fcppt/math/vector/vector.hpp>
#include <cmath>

namespace fruitcut
{
namespace math
{
namespace triangle
{
template<typename Triangle>
typename triangle::scalar_type<Triangle>::type
area(
	Triangle const &t)
{
	typename triangle::vector_type<Triangle>::type 
		ab =
			triangle::vertex_access<Triangle>::get(t,1) - 
			triangle::vertex_access<Triangle>::get(t,0),
		ac = 
			triangle::vertex_access<Triangle>::get(t,2) - 
			triangle::vertex_access<Triangle>::get(t,0);

	return 
		static_cast<typename triangle::scalar_type<Triangle>::type >(
			0.5) * 
		std::abs(
			ab.x() * ac.y() - ac.x() * ab.y());
}
}
}
}

#endif