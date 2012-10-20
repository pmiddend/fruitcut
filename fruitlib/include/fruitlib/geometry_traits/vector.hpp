#ifndef FRUITLIB_GEOMETRY_TRAITS_VECTOR_HPP_INCLUDED
#define FRUITLIB_GEOMETRY_TRAITS_VECTOR_HPP_INCLUDED

#include <fcppt/math/vector/object_impl.hpp>
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
template<typename T,typename N,typename S>
struct tag<fcppt::math::vector::object<T,N,S> >
{
	typedef point_tag type;
};

template<typename T,typename N,typename S>
struct coordinate_type<fcppt::math::vector::object<T,N,S> >
{
	typedef T type;
};

template<typename T,typename N,typename S>
struct coordinate_system<fcppt::math::vector::object<T,N,S> >
{
	typedef boost::geometry::cs::cartesian type;
};

FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

template<typename T,typename N,typename S>
struct dimension<fcppt::math::vector::object<T,N,S> >
	: N
{};

FCPPT_PP_POP_WARNING

template<typename T,typename N,typename S,std::size_t Dim>
struct access<fcppt::math::vector::object<T,N,S>,Dim>
{
	static T
	get(
		fcppt::math::vector::object<T,N,S> const &p)
	{
			return p[Dim];
	}

	static void
	set(
		fcppt::math::vector::object<T,N,S> &p,
		T const& value)
	{
			p[Dim] = value;
	}
};

}
}
}

#endif
