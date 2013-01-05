#include <fruitlib/texture_manager.hpp>
#include <sge/image2d/file.hpp>
#include <sge/image2d/system.hpp>
#include <sge/renderer/texture/color_format.hpp>
#include <sge/renderer/texture/create_planar_from_path.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <fcppt/make_unique_ptr.hpp>


fruitlib::texture_manager::texture_manager(
	sge::renderer::device::core &_renderer,
	sge::image2d::system &_image_system,
	sge::renderer::texture::emulate_srgb const _emulate_srgb)
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

sge::texture::fragmented_unique_ptr
fruitlib::texture_manager::create_rect_fragmented(
	sge::renderer::texture::mipmap::object const &_mipmap,
	sge::image::color::format const _color_format,
	sge::renderer::dim2 const &_size)
{
	return
		sge::texture::fragmented_unique_ptr(
			fcppt::make_unique_ptr<sge::texture::rect_fragmented>(
				renderer_,
				sge::renderer::texture::color_format(
					_color_format,
					emulate_srgb_),
				_mipmap,
				_size));
}

sge::image2d::file_unique_ptr
fruitlib::texture_manager::create_image_from_path(
	boost::filesystem::path const &_path)
{
	return
		image_system_.load(
			_path);
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

sge::renderer::texture::emulate_srgb
fruitlib::texture_manager::emulate_srgb() const
{
	return
		emulate_srgb_;
}

fruitlib::texture_manager::~texture_manager()
{
}
