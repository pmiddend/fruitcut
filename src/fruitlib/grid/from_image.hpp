#ifndef FRUITLIB_GRID_FROM_IMAGE_HPP_INCLUDED
#define FRUITLIB_GRID_FROM_IMAGE_HPP_INCLUDED

#include <sge/image/raw_pointer.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/file_ptr.hpp>
#include <sge/image2d/algorithm/copy_and_convert.hpp>
#include <sge/image2d/view/make.hpp>
#include <sge/image2d/view/optional_pitch.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/container/grid/object.hpp>
#include <fcppt/math/dim/structure_cast.hpp>


namespace fruitlib
{
namespace grid
{
template<typename T>
fcppt::container::grid::object<T,2> const
from_image(
	sge::image2d::file_ptr const input_file,
	sge::image::color::format::type const f)
{
	typedef
	fcppt::container::grid::object<T,2>
	result_type;

	result_type result(
		fcppt::math::dim::structure_cast<typename result_type::dim>(
			input_file->size()));
	sge::image2d::algorithm::copy_and_convert(
		input_file->view(),
		sge::image2d::view::make(
			reinterpret_cast<sge::image::raw_pointer>(
				result.data()),
			result.size(),
			f,
			sge::image2d::view::optional_pitch()));
	return result;
}
}
}

#endif
