#ifndef FRUITLIB_PP_FILTER_BLUR_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_BLUR_HPP_INCLUDED

#include <fruitlib/pp/filter/unary.hpp>
#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <cstddef>

namespace fruitlib
{
namespace pp
{
namespace filter
{
class blur
:
	public unary
{
FCPPT_NONCOPYABLE(
	blur);
public:
	typedef
	std::size_t
	size_type;

	explicit	
	blur(
		sge::renderer::device &,
		filter::manager &,
		texture::manager &,
		sge::renderer::dim2 const &,
		size_type iterations);

	texture::counted_instance const
	apply(
		texture::counted_instance);

	~blur();
private:
	typedef
	fcppt::container::array<texture::counted_instance,2>
	instance_array;

	sge::renderer::device &renderer_;
	filter::manager &filter_manager_;
	texture::manager &texture_manager_;
	sge::renderer::dim2 const texture_size_;
	size_type const iterations_;

	fcppt::container::array<sge::shader::object*,2> shaders_;

	void
	render(
		instance_array &,
		size_type);
};
}
}
}

#endif
