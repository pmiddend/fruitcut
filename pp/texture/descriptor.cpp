#include "descriptor.hpp"
#include <fcppt/math/dim/comparison.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/range/algorithm/lexicographical_compare.hpp>

fruitcut::pp::texture::descriptor::descriptor(
	sge::renderer::dim2 const &_size,
	sge::image::color::format::type const _image_format,
	sge::renderer::texture::filter::object const &_filter,
	depth_stencil_format::type const _depth_stencil)
:
	tuple_(
		_size,
		_image_format,
		filter_wrapper(
			_filter),
		_depth_stencil)
{
}

sge::renderer::dim2 const &
fruitcut::pp::texture::descriptor::size() const
{
	return tuple_.get<0>();
}

sge::image::color::format::type 
fruitcut::pp::texture::descriptor::image_format() const
{
	return tuple_.get<1>();
}

sge::renderer::texture::filter::object const &
fruitcut::pp::texture::descriptor::filter() const
{
	return tuple_.get<2>().value();
}

fruitcut::pp::texture::depth_stencil_format::type 
fruitcut::pp::texture::descriptor::depth_stencil() const
{
	return tuple_.get<3>();
}

bool 
fruitcut::pp::texture::descriptor::operator<(
	descriptor const &r) const
{
	return 
		tuple_ < r.tuple_;
}
