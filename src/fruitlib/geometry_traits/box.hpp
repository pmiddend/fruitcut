#ifndef FRUITLIB_GEOMETRY_TRAITS_BOX_HPP_INCLUDED
#define FRUITLIB_GEOMETRY_TRAITS_BOX_HPP_INCLUDED

#include <fcppt/math/size_type.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <cstddef>
#include <fcppt/config/external_end.hpp>


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
			b.size();
		d[Dimension] =
			(p[Dimension] + d[Dimension]) - value;
		p[Dimension] =
			value;
		b.pos(
			p);
		b.size(
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
		return (b.pos() + b.size())[Dimension];
	}

	static inline void
	set(
		fcppt::math::box::basic<T,N> &b,
		T const& value)
	{
		typename fcppt::math::box::basic<T,N>::dim d =
			b.size();
		d[Dimension] =
			value - b.pos()[Dimension];
		b.size(
			d);
	}
};

}
}
}

#endif
