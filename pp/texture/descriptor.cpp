#include "descriptor.hpp"
#include <fcppt/math/dim/comparison.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/range/algorithm/lexicographical_compare.hpp>

fruitcut::pp::texture::descriptor::descriptor(
	sge::renderer::dim2 const &_size,
	sge::image::color::format::type const _image_format,
	sge::renderer::filter::texture const &_filter)
:
	tuple_(
		_size,
		_image_format,
		filter_wrapper(
			_filter))
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

sge::renderer::filter::texture const &
fruitcut::pp::texture::descriptor::filter() const
{
	return tuple_.get<2>().value();
}

bool 
fruitcut::pp::texture::descriptor::operator<(
	descriptor const &r) const
{
	return 
		tuple_ < r.tuple_;
}
