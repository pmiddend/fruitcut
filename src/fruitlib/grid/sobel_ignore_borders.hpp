#ifndef FRUITLIB_GRID_SOBEL_IGNORE_BORDERS_HPP_INCLUDED
#define FRUITLIB_GRID_SOBEL_IGNORE_BORDERS_HPP_INCLUDED

#include "zip_with.hpp"
#include "convolve_ignore_borders.hpp"
#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/object/static_cast.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <cmath>

namespace fruitlib
{
namespace grid
{
template<typename T>
fcppt::container::grid::object<T,2> const
sobel_ignore_borders(
	fcppt::container::grid::object<T,2> const &a)
{
	typedef typename
	fcppt::math::vector::static_<T,3>::type
	vector;

	vector f(1,0,-1),g(1,2,1);

	return 
		zip_with<T,T>(
			convolve_ignore_borders<T>(
				convolve_ignore_borders<T>(
					a,
					f,
					0u),
				g,
				1u),
			convolve_ignore_borders<T>(
				convolve_ignore_borders<T>(
					a,
					g,
					0u),
				f,
				1u),
			boost::phoenix::bind(
				static_cast<T (*)(T)>(&std::sqrt),
				boost::phoenix::arg_names::arg1 * boost::phoenix::arg_names::arg1 + 
				boost::phoenix::arg_names::arg2 * boost::phoenix::arg_names::arg2));
}

}
}

#endif
