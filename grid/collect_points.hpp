#ifndef FRUITCUT_GRID_COLLECT_POINTS_HPP_INCLUDED
#define FRUITCUT_GRID_COLLECT_POINTS_HPP_INCLUDED

#include <fcppt/container/grid/size_type.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/tr1/type_traits.hpp>
#include <boost/static_assert.hpp>

namespace fruitcut
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
	BOOST_STATIC_ASSERT((
		std::tr1::is_same
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
