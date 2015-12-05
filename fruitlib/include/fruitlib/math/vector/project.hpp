#ifndef FRUITLIB_MATH_VECTOR_PROJECT_HPP_INCLUDED
#define FRUITLIB_MATH_VECTOR_PROJECT_HPP_INCLUDED

#include <fcppt/math/vector/dot.hpp>


namespace fruitlib
{
namespace math
{
namespace vector
{

/**
\brief Project a vector orthogonally onto another vector
\tparam Vector A fcppt::math::vector::object type.
\param u The vector to project to
\param v The vector to project from

This is expressed as \f[ \textrm{project}(u,v) = \frac{\langle u,v \rangle}{\langle u,u \rangle} u \f]

See:

http://en.wikipedia.org/wiki/Gram_schmidt#The_Gram.E2.80.93Schmidt_process

for an explanation.
*/
template<typename Vector>
Vector
project(
	Vector const &u,
	Vector const &v)
{
	return fcppt::math::vector::dot(v,u)/fcppt::math::vector::dot(u,u) * u;
}
}
}
}

#endif
