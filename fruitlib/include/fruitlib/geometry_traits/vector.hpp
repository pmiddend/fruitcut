#ifndef FRUITLIB_GEOMETRY_TRAITS_VECTOR_HPP_INCLUDED
#define FRUITLIB_GEOMETRY_TRAITS_VECTOR_HPP_INCLUDED

#include <fruitlib/def_ctor.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/static_size.hpp>
#include <fcppt/math/vector/at.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
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
		fcppt::math::vector::static_<
			T,
			N
		>
	>
>
{
	typedef point_tag type;
};

template<
	typename T,
	fcppt::math::size_type N
>
struct coordinate_type<
	fruitlib::def_ctor<
		fcppt::math::vector::static_<
			T,
			N
		>
	>
>
{
	typedef T type;
};

template<
	typename T,
	fcppt::math::size_type N
>
struct coordinate_system<
	fruitlib::def_ctor<
		fcppt::math::vector::static_<
			T,
			N
		>
	>
>
{
	typedef boost::geometry::cs::cartesian type;
};

FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

template<
	typename T,
	fcppt::math::size_type N
>
struct dimension<
	fruitlib::def_ctor<
		fcppt::math::vector::static_<
			T,
			N
		>
	>
>
:
	fcppt::math::static_size<
		N
	>
{};

FCPPT_PP_POP_WARNING

template<
	typename T,
	fcppt::math::size_type N,
	std::size_t Dim
>
struct access<
	fruitlib::def_ctor<
		fcppt::math::vector::static_<
			T,
			N
		>
	>,
	Dim
>
{
	static
	T
	get(
		fruitlib::def_ctor<
			fcppt::math::vector::static_<
				T,
				N
			>
		> const &_vec
	)
	{
		return
			fcppt::math::vector::at<
				Dim
			>(
				_vec.base()
			);
	}

	static
	void
	set(
		fruitlib::def_ctor<
			fcppt::math::vector::static_<
				T,
				N
			>
		> &_vec,
		T const &_value
	)
	{
		fcppt::math::vector::at<
			Dim
		>(
			_vec.base()
		) =
			_value;
	}
};

}
}
}

#endif
