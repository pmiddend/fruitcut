#ifndef FRUITCUT_FRUITLIB_PP_TEXTURE_DESCRIPTOR_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_TEXTURE_DESCRIPTOR_HPP_INCLUDED

#include "filter_wrapper.hpp"
#include "depth_stencil_format.hpp"
#include <sge/renderer/dim2.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/texture/filter/object.hpp>
#include <sge/renderer/resource_flags_field.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <boost/fusion/tuple.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace pp
{
namespace texture
{
class descriptor
{
public:
	explicit
	descriptor(
		sge::renderer::dim2 const &,
		sge::image::color::format::type,
		sge::renderer::texture::filter::object const &,
		depth_stencil_format::type);

	sge::renderer::dim2 const &
	size() const;

	sge::image::color::format::type 
	image_format() const;

	sge::renderer::texture::filter::object const &
	filter() const;

	depth_stencil_format::type
	depth_stencil() const;

	bool 
	operator<(
		descriptor const &) const;
private:
	typedef 
	boost::fusion::tuple
	<
		sge::renderer::dim2,
		sge::image::color::format::type,
		filter_wrapper,
		depth_stencil_format::type
	>
	tuple_type;

	tuple_type tuple_;
};
}
}
}
}

#endif
