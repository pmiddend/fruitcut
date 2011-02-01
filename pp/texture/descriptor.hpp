#ifndef FRUITCUT_PP_TEXTURE_DESCRIPTOR_HPP_INCLUDED
#define FRUITCUT_PP_TEXTURE_DESCRIPTOR_HPP_INCLUDED

#include "filter_wrapper.hpp"
#include <sge/renderer/dim2.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/texture.hpp>
#include <sge/renderer/resource_flags_field.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <boost/tuple/tuple.hpp>

namespace fruitcut
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
		sge::renderer::filter::texture const &);

	sge::renderer::dim2 const &
	size() const;

	sge::image::color::format::type 
	image_format() const;

	sge::renderer::filter::texture const &
	filter() const;

	bool 
	operator<(
		descriptor const &) const;
private:
	typedef 
	boost::tuple
	<
		sge::renderer::dim2,
		sge::image::color::format::type,
		filter_wrapper
	>
	tuple_type;

	tuple_type tuple_;
};
}
}
}

#endif