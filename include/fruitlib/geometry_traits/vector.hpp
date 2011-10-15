#ifndef FRUITLIB_GEOMETRY_TRAITS_VECTOR_HPP_INCLUDED
#define FRUITLIB_GEOMETRY_TRAITS_VECTOR_HPP_INCLUDED

#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/geometry/geometry.hpp>
#include <cstddef>

namespace boost
{
namespace geometry
{
namespace traits
{
template<typename T,typename N,typename S>
struct tag<fcppt::math::vector::basic<T,N,S> >
{
	typedef point_tag type;
};

template<typename T,typename N,typename S>
struct coordinate_type<fcppt::math::vector::basic<T,N,S> >
{
	typedef T type;
};

template<typename T,typename N,typename S>
struct coordinate_system<fcppt::math::vector::basic<T,N,S> >
{
	typedef boost::geometry::cs::cartesian type;
};

template<typename T,typename N,typename S>
struct dimension<fcppt::math::vector::basic<T,N,S> >
	: N
{};

template<typename T,typename N,typename S,std::size_t Dim>
struct access<fcppt::math::vector::basic<T,N,S>,Dim>
{
	static T
	get(
		fcppt::math::vector::basic<T,N,S> const &p)
	{
			return p[Dim];
	}

	static void
	set(
		fcppt::math::vector::basic<T,N,S> &p,
		T const& value)
	{
			p[Dim] = value;
	}
};

}
}
}

#endif
