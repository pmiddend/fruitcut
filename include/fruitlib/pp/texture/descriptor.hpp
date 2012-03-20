#ifndef FRUITLIB_PP_TEXTURE_DESCRIPTOR_HPP_INCLUDED
#define FRUITLIB_PP_TEXTURE_DESCRIPTOR_HPP_INCLUDED

#include <fruitlib/pp/texture/depth_stencil_format.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/resource_flags_field.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/fusion/tuple.hpp>
#include <fcppt/config/external_end.hpp>


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
		depth_stencil_format::type);

	sge::renderer::dim2 const &
	size() const;

	sge::image::color::format::type
	image_format() const;

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
		depth_stencil_format::type
	>
	tuple_type;

	tuple_type tuple_;
};
}
}
}

#endif
