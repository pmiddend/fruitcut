#ifndef FRUITCUT_GEOMETRY_TRAITS_BOX_HPP_INCLUDED
#define FRUITCUT_GEOMETRY_TRAITS_BOX_HPP_INCLUDED

#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/geometry/geometry.hpp>
#include <cstddef>

namespace boost 
{ 
namespace geometry 
{ 
namespace traits 
{

template<typename T,fcppt::math::size_type N>
struct tag<fcppt::math::box::basic<T,N> >
{
	typedef box_tag type;
};

template<typename T,fcppt::math::size_type N>
struct point_type< fcppt::math::box::basic<T,N> >
{
    typedef typename fcppt::math::box::basic<T,N>::vector type;
};

template<typename T,fcppt::math::size_type N, std::size_t Dimension>
struct indexed_access<fcppt::math::box::basic<T,N>, boost::geometry::min_corner, Dimension>
{
	static inline T 
	get(
		fcppt::math::box::basic<T,N> const &b)
	{
		return b.pos()[Dimension];
	}

	static inline void 
	set(
		fcppt::math::box::basic<T,N> &b, 
		T const& value)
	{
		typename fcppt::math::box::basic<T,N>::vector p = 
			b.pos();
		typename fcppt::math::box::basic<T,N>::dim d = 
			b.dimension();
		d[Dimension] = 
			(p[Dimension] + d[Dimension]) - value;
		p[Dimension] = 
			value;
		b.pos(
			p);
		b.dimension(
			d);
	}
};

template<typename T,fcppt::math::size_type N, std::size_t Dimension>
struct indexed_access<fcppt::math::box::basic<T,N>, boost::geometry::max_corner, Dimension>
{
	static inline T 
	get(
		fcppt::math::box::basic<T,N> const &b)
	{
		return (b.pos() + b.dimension())[Dimension];
	}

	static inline void 
	set(
		fcppt::math::box::basic<T,N> &b, 
		T const& value)
	{
		typename fcppt::math::box::basic<T,N>::dim d = 
			b.dimension();
		d[Dimension] = 
			value - b.pos()[Dimension];
		b.dimension(
			d);
	}
};

}
}
}

#endif
