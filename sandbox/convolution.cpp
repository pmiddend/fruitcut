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
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/object/static_cast.hpp>
#include <boost/spirit/home/phoenix/operator/arithmetic.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/promote.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <limits>
#include <iostream>

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
convolve_h_no_borders(
	fcppt::container::grid::object<T,2> const &g,
	typename fcppt::math::vector::static_<T,3>::type const &v)
{
	typedef
	fcppt::container::grid::object<T,2>
	grid;

	typedef typename
	grid::dim
	dim;

	typedef typename
	grid::size_type
	size_type;

	grid result(
		g.dimension(),
		static_cast<T>(0));

	FCPPT_ASSERT(
		v.size() % static_cast<size_type>(2));

	FCPPT_ASSERT(
		g.dimension().w() >= v.size());

	FCPPT_ASSERT(
		!v.empty());

	size_type const distance = 
		static_cast<size_type>(
			(v.size()-1)/2);

	for (dim p(0,0); p.h() < g.dimension().h(); ++p.h())
	{
		for (p.w() = 0; p.w() < g.dimension().w(); ++p.w())
		{
			if (p.w() < distance || p.w() >= static_cast<size_type>(g.dimension().w() - distance))
			{
				result[p] = g[p];
				continue;
			}

			for(size_type x = 0; x < v.size(); ++x)
				result[p] += 
					v[x] * 
					g[
						dim(
							static_cast<size_type>(
								p.w() - distance + x),
							p.h())];
		}
	}

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
			boost::phoenix::arg_names::arg1 * std::numeric_limits<gray_grid::value_type>::max()));
	return g;
}

scalar_grid const
sobel(
	scalar_grid const &a)
{
	scalar_grid const result = 
		convolve_h_no_borders<scalar>(
			a,
			fcppt::math::vector::static_<scalar,3>::type(
				1,
				0,
				-1));
	return result;
}

template<typename Char>
void
gray_grid_to_ppm(
	gray_grid const &g,
	std::basic_ostream<Char> &s)
{
	BOOST_STATIC_ASSERT(
		boost::is_unsigned<gray_grid::value_type>::value);

	typedef
	boost::promote<gray_grid::value_type>::type
	numeric_type;

	s 
		<< s.widen('P') << s.widen('2') << s.widen('\n')
		<< g.dimension().w() << s.widen(' ') << g.dimension().h() << s.widen('\n')
		<< static_cast<numeric_type>(std::numeric_limits<gray_grid::value_type>::max()) << s.widen('\n');

	for (gray_grid::dim d(0,0); d.h() < g.dimension().h(); ++d.h())
	{
		for (d.w() = 0; d.w() < g.dimension().w(); ++d.w())
			s << static_cast<numeric_type>(g[d]) << s.widen(' ');
		s << s.widen('\n');
	}
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

	gray_grid const g = 
		scalar_grid_to_gray_grid(
			sobel(
				gray_grid_to_scalar_grid(
					image_to_gray_grid(
						sys.image_loader().load(
							fruitcut::media_path()
								/ FCPPT_TEXT("textures")
								/ FCPPT_TEXT("logo.png"))))));

	gray_grid_to_ppm(
		g,
		std::cout);
}
