#ifndef FRUITLIB_JSON_DETAIL_IS_ARRAY_HPP_INCLUDED
#define FRUITLIB_JSON_DETAIL_IS_ARRAY_HPP_INCLUDED

#include <fcppt/container/array.hpp>
#include <fcppt/tr1/array.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/detail/static_storage.hpp>
#include <fcppt/math/dim/static.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/array.hpp>
#include <cstddef>

namespace fruitlib
{
namespace json
{
namespace detail
{
template<typename T>
struct is_array : boost::mpl::false_ {};

template<typename T,std::size_t N>
struct is_array< fcppt::container::array<T,N> > : boost::mpl::true_ {};

template<typename T,std::size_t N>
struct is_array< std::tr1::array<T,N> > : boost::mpl::true_ {};

template<typename T,std::size_t N>
struct is_array< boost::array<T,N> > : boost::mpl::true_ {};

template<typename T,fcppt::math::size_type N>
struct is_array
< 
	fcppt::math::vector::basic
	<
		T,
		boost::mpl::integral_c
		<
			fcppt::math::size_type,
			N
		>,
		typename 
		fcppt::math::detail::static_storage
		<
			T,
			N
		>::type
	> 
> : boost::mpl::true_ {};

template<typename T,fcppt::math::size_type N>
struct is_array
< 
	fcppt::math::dim::basic
	<
		T,
		boost::mpl::integral_c
		<
			fcppt::math::size_type,
			N
		>,
		typename 
		fcppt::math::detail::static_storage
		<
			T,
			N
		>::type
	> 
> : boost::mpl::true_ {};
}
}
}

#endif
