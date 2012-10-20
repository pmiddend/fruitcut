#ifndef FRUITLIB_GRID_TO_PPM_HPP_INCLUDED
#define FRUITLIB_GRID_TO_PPM_HPP_INCLUDED

#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/tr1/type_traits.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/type_traits/promote.hpp>
#include <boost/utility/enable_if.hpp>
#include <limits>
#include <ostream>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace grid
{
template<typename Char,typename T>
typename
boost::enable_if_c
<
	std::tr1::is_unsigned<T>::value,
	void
>::type
to_ppm(
	fcppt::container::grid::object<T,2> const &g,
	std::basic_ostream<Char> &s,
	T const max_value = std::numeric_limits<T>::max())
{
	typedef
	fcppt::container::grid::object<T,2>
	grid;

	typedef typename
	boost::promote<typename grid::value_type>::type
	numeric_type;

	s
		<< s.widen('P') << s.widen('2') << s.widen('\n')
		<< g.size().w() << s.widen(' ') << g.size().h() << s.widen('\n')
		<< static_cast<numeric_type>(max_value) << s.widen('\n');

	for (typename grid::dim d(0,0); d.h() < g.size().h(); ++d.h())
	{
		for (d.w() = 0; d.w() < g.size().w(); ++d.w())
			s << static_cast<numeric_type>(g[d]) << s.widen(' ');
		s << s.widen('\n');
	}
}
}
}

#endif
