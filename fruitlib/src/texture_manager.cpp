#include <fruitlib/texture_manager.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/planar.hpp>


fruitlib::texture_manager::texture_manager(
	sge::renderer::device::core &_renderer,
	sge::image2d::system &_image_system,
	sge::renderer::texture::emulate_srgb::type const _emulate_srgb)
:
	renderer_(
		_renderer),
	image_system_(
		_image_system),
	emulate_srgb_(
		_emulate_srgb)
{
}

sge::renderer::texture::planar_unique_ptr
fruitlib::texture_manager::create_planar_from_path(
	boost::filesystem::path const &_path,
	sge::renderer::texture::mipmap::object const &_mipmap,
	sge::renderer::resource_flags_field const &_resource_flags)
{
	return
		sge::renderer::texture::create_planar_from_path(
			_path,
			renderer_,
			image_system_,
			_mipmap,
			_resource_flags,
			emulate_srgb_);
}

sge::renderer::texture::planar_unique_ptr
fruitlib::texture_manager::create_planar_from_view(
	sge::image2d::view::const_object const &_view,
	sge::renderer::texture::mipmap::object const &_mipmap,
	sge::renderer::resource_flags_field const &_resource_flags)
{
	return
		sge::renderer::texture::create_planar_from_view(
			renderer_,
			_view,
			_mipmap,
			_resource_flags,
			emulate_srgb_);
}

sge::renderer::device::core &
fruitlib::texture_manager::renderer() const
{
	return
		renderer_;
}

sge::image2d::system &
fruitlib::texture_manager::image_system() const
{
	return
		image_system_;
}

sge::renderer::texture::emulate_srgb::type
fruitlib::texture_manager::emulate_srgb() const
{
	return
		emulate_srgb_;
}

fruitlib::texture_manager::~texture_manager()
{
}
