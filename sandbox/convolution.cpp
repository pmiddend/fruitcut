#include "../media_path.hpp"
#include "../grid/from_image.hpp"
#include "../grid/to_ppm.hpp"
#include "../grid/map.hpp"
#include "../grid/sobel_ignore_borders.hpp"
#include "../grid/collect_points.hpp"
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


scalar_grid const
gray_grid_to_scalar_grid(
	gray_grid const &input)
{
	return 
		fruitcut::grid::map<scalar_grid::value_type>(
			input,
			boost::phoenix::static_cast_<scalar_grid::value_type>(
			boost::phoenix::arg_names::arg1 - std::numeric_limits<gray_grid::value_type>::min())/
			static_cast<scalar_grid::value_type>(
				std::numeric_limits<gray_grid::value_type>::max() - std::numeric_limits<gray_grid::value_type>::min()));
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
	
	scalar_grid const g = 
		fruitcut::grid::map<scalar>(
			fruitcut::grid::sobel_ignore_borders(
				gray_grid_to_scalar_grid(
					fruitcut::grid::from_image<gray_grid::value_type>(
						sys.image_loader().load(
							fruitcut::media_path()
								/ FCPPT_TEXT("textures")
								/ FCPPT_TEXT("logo.png")),
						sge::image::color::format::gray8))),
			boost::bind(
				&fcppt::math::clamp<scalar>,
				_1,
				static_cast<scalar>(0),
				static_cast<scalar>(1)));

	typedef
	std::vector<scalar_grid::dim>
	point_sequence;

	point_sequence points = 
		fruitcut::grid::collect_points<point_sequence>(
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

	fruitcut::grid::to_ppm(
		ints,
		std::cout,
		255u);
}
