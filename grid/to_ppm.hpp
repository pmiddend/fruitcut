#ifndef FRUITCUT_GRID_TO_PPM_HPP_INCLUDED
#define FRUITCUT_GRID_TO_PPM_HPP_INCLUDED

#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/promote.hpp>
#include <ostream>
#include <limits>

namespace fruitcut
{
namespace grid
{
template<typename Char,typename T>
typename
boost::enable_if_c<boost::is_unsigned<T>::value,void>::type
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
		<< g.dimension().w() << s.widen(' ') << g.dimension().h() << s.widen('\n')
		<< static_cast<numeric_type>(max_value) << s.widen('\n');

	for (typename grid::dim d(0,0); d.h() < g.dimension().h(); ++d.h())
	{
		for (d.w() = 0; d.w() < g.dimension().w(); ++d.w())
			s << static_cast<numeric_type>(g[d]) << s.widen(' ');
		s << s.widen('\n');
	}
}
}
}

#endif
