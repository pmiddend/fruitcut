#ifndef FRUITLIB_GRID_MAP_HPP_INCLUDED
#define FRUITLIB_GRID_MAP_HPP_INCLUDED

#include <fcppt/container/grid/object.hpp>
#include <fcppt/container/grid/size_type.hpp>
#include <fcppt/config/external_begin.hpp>
#include <algorithm>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace grid
{
template
<
	typename Dest,
	typename Source,
	fcppt::container::grid::size_type N,
	typename Functor
>
fcppt::container::grid::object<Dest,N> const
map(
	fcppt::container::grid::object<Source,N> const &g,
	Functor const &f)
{
	fcppt::container::grid::object<Dest,N> result(
		g.size());
	std::transform(
		g.begin(),
		g.end(),
		result.begin(),
		f);
	return result;
}
}
}

#endif
