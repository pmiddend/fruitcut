#include "../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/algorithm/copy_and_convert.hpp>
#include <sge/image2d/view/optional_pitch.hpp>
#include <sge/image2d/view/make.hpp>
#include <sge/image/color/channel8.hpp>
#include <sge/image/raw_pointer.hpp>
#include <sge/extension_set.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/clamp.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/comparison.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/object/static_cast.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/promote.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <limits>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace
{
typedef
float
scalar;

typedef
fcppt::container::grid::object
<
	sge::image::color::channel8,
	2
>
gray_grid;

typedef
fcppt::container::grid::object
<
	scalar,
	2
>
scalar_grid;

gray_grid const
image_to_gray_grid(
	sge::image2d::file_ptr const input_file)
{
	gray_grid output(
		fcppt::math::dim::structure_cast<gray_grid::dim>(
			input_file->dim()));
	sge::image2d::algorithm::copy_and_convert(
		input_file->view(),
		sge::image2d::view::make(
			reinterpret_cast<sge::image::raw_pointer>(
				output.data()),
			output.dimension(),
			sge::image::color::format::gray8,
			sge::image2d::view::optional_pitch()));
	return output;
}

scalar_grid const
gray_grid_to_scalar_grid(
	gray_grid const &input)
{
	scalar_grid g(
		input.dimension());

	std::transform(
		input.begin(),
		input.end(),
		g.begin(),
		boost::phoenix::static_cast_<scalar_grid::value_type>(
			boost::phoenix::arg_names::arg1 - std::numeric_limits<gray_grid::value_type>::min())/
		static_cast<scalar_grid::value_type>(
			std::numeric_limits<gray_grid::value_type>::max() - std::numeric_limits<gray_grid::value_type>::min()));
	return g;
}

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
		g.dimension(),
		static_cast<T>(0));

	FCPPT_ASSERT(
		static_cast<size_type>(v.size()) % static_cast<size_type>(2));

	FCPPT_ASSERT(
		g.dimension()[axis] >= static_cast<dim_value_type>(v.size()));

	FCPPT_ASSERT(
		!v.empty());

	dim_value_type const distance = 
		static_cast<dim_value_type>(
			(v.size()-1)/2);

	for (dim p(0,0); p[caxis] < g.dimension()[caxis]; ++p[caxis])
	{
		for (p[axis] = 0; p[axis] < g.dimension()[axis]; ++p[axis])
		{
			if (p[axis] < distance || p[axis] >= static_cast<dim_value_type>(g.dimension()[axis] - distance))
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

template
<
	typename Dest,
	typename Source,
	fcppt::container::grid::size_type N,
	typename Functor>
fcppt::container::grid::object<Dest,N> const
grid_map(
	fcppt::container::grid::object<Source,N> const &g,
	Functor const &f)
{
	fcppt::container::grid::object<Source,N> result(
		g.dimension());
	std::transform(
		g.begin(),
		g.end(),
		result.begin(),
		f);
	return result;
}

template
<
	typename Source,
	typename Dest,
	fcppt::container::grid::size_type N,
	typename Functor>
fcppt::container::grid::object<Dest,N> const
grid_map(
	fcppt::container::grid::object<Source,N> const &g,
	fcppt::container::grid::object<Source,N> const &h,
	Functor const &f)
{
	FCPPT_ASSERT(
		g.dimension() == h.dimension());
	fcppt::container::grid::object<Source,N> result(
		g.dimension());
	std::transform(
		g.begin(),
		g.end(),
		h.begin(),
		result.begin(),
		f);
	return result;
}

gray_grid const
scalar_grid_to_gray_grid(
	scalar_grid const &input)
{
	// This could easily be fixed
	BOOST_STATIC_ASSERT(
		boost::is_unsigned<gray_grid::value_type>::value);

	gray_grid g(
		input.dimension());
	std::transform(
		input.begin(),
		input.end(),
		g.begin(),
		boost::phoenix::static_cast_<gray_grid::value_type>(
			boost::phoenix::arg_names::arg1 
				* std::numeric_limits<gray_grid::value_type>::max()));
	return g;
}

scalar_grid const
sobel(
	scalar_grid const &a)
{
	return 
		grid_map<scalar,scalar>(
			convolve_ignore_borders<scalar>(
				convolve_ignore_borders<scalar>(
					a,
					fcppt::math::vector::static_<scalar,3>::type(
						1,
						0,
						-1),
					0u),
				fcppt::math::vector::static_<scalar,3>::type(
					1,
					2,
					1),
				1u),
			convolve_ignore_borders<scalar>(
				convolve_ignore_borders<scalar>(
					a,
					fcppt::math::vector::static_<scalar,3>::type(
						1,
						2,
						1),
					0u),
				fcppt::math::vector::static_<scalar,3>::type(
					1,
					0,
					-1),
				1u),
			boost::phoenix::bind(
				static_cast<scalar (*)(scalar)>(&std::sqrt),
				boost::phoenix::arg_names::arg1 * boost::phoenix::arg_names::arg1 + 
				boost::phoenix::arg_names::arg2 * boost::phoenix::arg_names::arg2));
}

template<typename Char,typename T>
typename
boost::enable_if_c<boost::is_unsigned<T>::value,void>::type
grid_to_ppm(
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

// After testing it, generalize to N dimensions (using recursion)
template<typename Points,typename T,typename UnaryFunctor>
Points const
collect_points(
	fcppt::container::grid::object<T,2> const &g,
	UnaryFunctor const &test)
{
	BOOST_STATIC_ASSERT((
		boost::is_same
		<
			typename Points::value_type,
			typename fcppt::container::grid::object<T,2>::dim
		>::value));

	Points result;
	for (typename fcppt::container::grid::object<T,2>::dim p(0,0); p.h() < g.dimension().h(); ++p.h())
		for (p.w() = 0; p.w() < g.dimension().w(); ++p.w())
			if(test(g[p]))
				result.push_back(
					p);
	return result;
}
}

int main()
{
	sge::systems::instance sys(
		sge::systems::list(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>(
					FCPPT_TEXT("png")))));

	/*
	gray_grid const g = 
		scalar_grid_to_gray_grid(
			grid_map<scalar>(
				sobel(
					gray_grid_to_scalar_grid(
						image_to_gray_grid(
							sys.image_loader().load(
								fruitcut::media_path()
									/ FCPPT_TEXT("textures")
									/ FCPPT_TEXT("logo.png"))))),
				boost::bind(
					&fcppt::math::clamp<scalar>,
					_1,
					static_cast<scalar>(0),
					static_cast<scalar>(1))));

	gray_grid_to_ppm(
		g,
		std::cout);
	*/
	
	scalar_grid const g = 
		grid_map<scalar>(
			sobel(
				gray_grid_to_scalar_grid(
					image_to_gray_grid(
						sys.image_loader().load(
							fruitcut::media_path()
								/ FCPPT_TEXT("textures")
								/ FCPPT_TEXT("logo.png"))))),
			boost::bind(
				&fcppt::math::clamp<scalar>,
				_1,
				static_cast<scalar>(0),
				static_cast<scalar>(1)));

	typedef
	std::vector<scalar_grid::dim>
	point_sequence;

	point_sequence points = 
		collect_points<point_sequence>(
			g,
			boost::phoenix::arg_names::arg1 > static_cast<scalar>(0.5));

	std::random_shuffle(
		points.begin(),
		points.end());

	points.resize(200);

	typedef
	fcppt::container::grid::object
	<
		unsigned,
		2	
	>
	uint_grid;

	uint_grid ints(
		g.dimension(),
		static_cast<unsigned>(0));

	BOOST_FOREACH(
		point_sequence::const_reference r,
		points)
		ints[r] = 255u;

	grid_to_ppm(
		ints,
		std::cout,
		255u);
}
