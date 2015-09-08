#ifndef FRUITLIB_GEOMETRY_TRAITS_BOX_HPP_INCLUDED
#define FRUITLIB_GEOMETRY_TRAITS_BOX_HPP_INCLUDED

#include <fruitlib/def_ctor.hpp>
#include <fcppt/math/at_c.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
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

template<
	typename T,
	fcppt::math::size_type N
>
struct tag<
	fruitlib::def_ctor<
		fcppt::math::box::object<
			T,
			N
		>
	>
>
{
	typedef box_tag type;
};

template<
	typename T,
	fcppt::math::size_type N
>
struct point_type<
	fruitlib::def_ctor<
		fcppt::math::box::object<
			T,
			N
		>
	>
>
{
	typedef
	fruitlib::def_ctor<
		typename
		fcppt::math::box::object<
			T,
			N
		>::vector
	>
	type;
};

template<
	typename T,
	fcppt::math::size_type N,
	std::size_t Dimension
>
struct indexed_access<
	fruitlib::def_ctor<
		fcppt::math::box::object<
			T,
			N
		>
	>,
	boost::geometry::min_corner,
	Dimension
>
{
	static
	T
	get(
		fruitlib::def_ctor<
			fcppt::math::box::object<
				T,
				N
			>
		> const &_box
	)
	{
		return
			fcppt::math::at_c<
				Dimension
			>(
				_box.pos()
			);
	}

	static
	void
	set(
		fruitlib::def_ctor<
			fcppt::math::box::object<
				T,
				N
			>
		> &_box,
		T const &_value
	)
	{
		fcppt::math::at_c<
			Dimension
		>(
			_box.pos()
		) =
			_value;
	}
};

template<
	typename T,
	fcppt::math::size_type N,
	std::size_t Dimension
>
struct indexed_access<
	fruitlib::def_ctor<
		fcppt::math::box::object<
			T,
			N
		>
	>,
	boost::geometry::max_corner,
	Dimension
>
{
	static
	T
	get(
		fruitlib::def_ctor<
			fcppt::math::box::object<
				T,
				N
			>
		> const &_box
	)
	{
		return
			fcppt::math::at_c<
				Dimension
			>(
				_box.max()
			);
	}

	static
	void
	set(
		fruitlib::def_ctor<
			fcppt::math::box::object<
				T,
				N
			>
		> &_box,
		T const &_value
	)
	{
		fcppt::math::at_c<
			Dimension
		>(
			_box.max()
		) =
			_value;
	}
};

}
}
}

#endif
