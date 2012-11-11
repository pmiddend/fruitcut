#ifndef FRUITLIB_GRID_COLLECT_POINTS_HPP_INCLUDED
#define FRUITLIB_GRID_COLLECT_POINTS_HPP_INCLUDED

#include <fcppt/static_assert_expression.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/container/grid/size_type.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/type_traits/is_same.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace grid
{
// TODO: generalize to N dimensions
template
<
	typename DestContainer,
	typename T,
	fcppt::container::grid::size_type N,
	typename UnaryFunctor
>
DestContainer const
collect_points(
	fcppt::container::grid::object<T,N> const &g,
	UnaryFunctor const &test)
{
	FCPPT_STATIC_ASSERT_EXPRESSION((
		boost::is_same
		<
			typename DestContainer::value_type,
			typename fcppt::container::grid::object<T,N>::dim
		>::value));

	DestContainer result;
	for (typename fcppt::container::grid::object<T,N>::dim p(0,0); p.h() < g.size().h(); ++p.h())
		for (p.w() = 0; p.w() < g.size().w(); ++p.w())
			if(test(g[p]))
				result.push_back(
					p);
	return result;
}
}
}

#endif
