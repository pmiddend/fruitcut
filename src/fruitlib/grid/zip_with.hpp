#ifndef FRUITLIB_GRID_ZIP_WITH_HPP_INCLUDED
#define FRUITLIB_GRID_ZIP_WITH_HPP_INCLUDED

#include <fcppt/container/grid/size_type.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/assert/pre.hpp>
#include <algorithm>

namespace fruitlib
{
namespace grid
{
template
<
	typename Source,
	typename Dest,
	fcppt::container::grid::size_type N,
	typename Functor
>
fcppt::container::grid::object<Dest,N> const
zip_with(
	fcppt::container::grid::object<Source,N> const &g,
	fcppt::container::grid::object<Source,N> const &h,
	Functor const &f)
{
	FCPPT_ASSERT_PRE(
		g.size() == h.size());
	fcppt::container::grid::object<Dest,N> result(
		g.size());
	std::transform(
		g.begin(),
		g.end(),
		h.begin(),
		result.begin(),
		f);
	return result;
}
}
}

#endif
