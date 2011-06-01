#ifndef FRUITCUT_FRUITLIB_PP_FILTER_BLUR_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_FILTER_BLUR_HPP_INCLUDED

#include "unary.hpp"
#include "../screen_vf/quad.hpp"
#include "../texture/manager_fwd.hpp"
#include "../texture/counted_instance.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/shader/object_ptr.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <cstddef>

namespace fruitcut
{
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
		fcppt::filesystem::path const &,
		sge::renderer::device &,
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
	texture::manager &texture_manager_;
	sge::renderer::dim2 const texture_size_;
	size_type const iterations_;

	fcppt::container::array<sge::shader::object_ptr,2> shaders_;
	boost::ptr_array<screen_vf::quad,2> quads_;

	void
	render(
		instance_array &,
		size_type);
};
}
}
}
}

#endif
