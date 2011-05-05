#ifndef FRUITCUT_GRID_CONVOLVE_IGNORE_BORDERS_HPP_INCLUDED
#define FRUITCUT_GRID_CONVOLVE_IGNORE_BORDERS_HPP_INCLUDED

#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/assert.hpp>

namespace fruitcut
{
namespace grid
{
template<typename T>
fcppt::container::grid::object<T,2> const
convolve_ignore_borders(
	fcppt::container::grid::object<T,2> const &g,
	typename fcppt::math::vector::static_<T,3>::type const &v,
	typename fcppt::container::grid::object<T,2>::dim::size_type const axis)
{
	typedef
	fcppt::container::grid::object<T,2>
	grid;

	typedef typename
	fcppt::math::vector::static_<T,3>::type
	vector;

	typedef typename
	grid::dim
	dim;

	typedef typename
	grid::dim::size_type
	size_type;

	typedef typename
	grid::dim::value_type
	dim_value_type;

	// The "other axis" or "counter axis" as I call it :>
	size_type const caxis = 
		static_cast<size_type>(
			1 - axis);

	grid result(
		g.size(),
		static_cast<T>(0));

	FCPPT_ASSERT(
		static_cast<size_type>(v.size()) % static_cast<size_type>(2));

	FCPPT_ASSERT(
		g.size()[axis] >= static_cast<dim_value_type>(v.size()));

	FCPPT_ASSERT(
		!v.empty());

	dim_value_type const distance = 
		static_cast<dim_value_type>(
			(v.size()-1)/2);

	for (dim p(0,0); p[caxis] < g.size()[caxis]; ++p[caxis])
	{
		for (p[axis] = 0; p[axis] < g.size()[axis]; ++p[axis])
		{
			if (p[axis] < distance || p[axis] >= static_cast<dim_value_type>(g.size()[axis] - distance))
			{
				result[p] = g[p];
				continue;
			}

			for(dim_value_type x = 0; x < static_cast<dim_value_type>(v.size()); ++x)
			{
				dim newp;
				newp[axis] = 
					static_cast<dim_value_type>(
						p[axis] - distance + x);
				newp[caxis] = p[caxis];

				result[p] += 
					v[static_cast<typename vector::size_type>(x)] * 
					g[newp];
			}
		}
	}

	return result;
}

}
}

#endif
