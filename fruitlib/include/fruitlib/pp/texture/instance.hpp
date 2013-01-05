#ifndef FRUITLIB_PP_TEXTURE_INSTANCE_HPP_INCLUDED
#define FRUITLIB_PP_TEXTURE_INSTANCE_HPP_INCLUDED

#include <fruitlib/pp/texture/descriptor.hpp>
#include <fruitlib/pp/texture/is_locked.hpp>
#include <sge/renderer/depth_stencil_buffer/surface_scoped_ptr.hpp>
#include <sge/renderer/depth_stencil_buffer/surface_unique_ptr.hpp>
#include <sge/renderer/target/offscreen_scoped_ptr.hpp>
#include <sge/renderer/target/offscreen_unique_ptr.hpp>
#include <sge/renderer/texture/planar_scoped_ptr.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/renderer/texture/planar_unique_ptr.hpp>


namespace fruitlib
{
namespace pp
{
namespace texture
{
class instance
{
FCPPT_NONCOPYABLE(
	instance);
public:
	instance(
		fruitlib::pp::texture::descriptor const &,
		sge::renderer::texture::planar_unique_ptr,
		sge::renderer::target::offscreen_unique_ptr,
		sge::renderer::depth_stencil_buffer::surface_unique_ptr,
		fruitlib::pp::texture::is_locked const &locked);

	sge::renderer::texture::planar_shared_ptr
	texture();

	sge::renderer::target::offscreen &
	target();

	bool
	locked() const;

	void
	locked(
		bool);

	fruitlib::pp::texture::descriptor const &
	descriptor() const;

	~instance();
private:
	fruitlib::pp::texture::descriptor descriptor_;
	sge::renderer::texture::planar_shared_ptr const texture_;
	sge::renderer::target::offscreen_scoped_ptr const target_;
	sge::renderer::depth_stencil_buffer::surface_scoped_ptr const depth_stencil_;
	bool locked_;
};
}
}
}

#endif
