#ifndef FRUITLIB_TEXTURE_MANAGER_HPP_INCLUDED
#define FRUITLIB_TEXTURE_MANAGER_HPP_INCLUDED

#include <sge/image/color/format.hpp>
#include <sge/image2d/file_unique_ptr.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/image2d/view/const_object_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/resource_flags_field.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <sge/renderer/texture/planar_unique_ptr.hpp>
#include <sge/renderer/texture/mipmap/object_fwd.hpp>
#include <sge/texture/fragmented_unique_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
class texture_manager
{
FCPPT_NONCOPYABLE(
	texture_manager);
public:
	texture_manager(
		sge::renderer::device::core &,
		sge::image2d::system &,
		sge::renderer::texture::emulate_srgb::type);

	sge::renderer::texture::planar_unique_ptr
	create_planar_from_path(
		boost::filesystem::path const &,
		sge::renderer::texture::mipmap::object const &,
		sge::renderer::resource_flags_field const &);

	sge::renderer::texture::planar_unique_ptr
	create_planar_from_view(
		sge::image2d::view::const_object const &,
		sge::renderer::texture::mipmap::object const &,
		sge::renderer::resource_flags_field const &);

	sge::texture::fragmented_unique_ptr
	create_rect_fragmented(
		sge::renderer::texture::mipmap::object const &,
		sge::image::color::format::type,
		sge::renderer::dim2 const &);

	sge::image2d::file_unique_ptr
	create_image_from_path(
		boost::filesystem::path const &);

	sge::renderer::device::core &
	renderer() const;

	sge::image2d::system &
	image_system() const;

	sge::renderer::texture::emulate_srgb::type
	emulate_srgb() const;

	~texture_manager();
private:
	sge::renderer::device::core &renderer_;
	sge::image2d::system &image_system_;
	sge::renderer::texture::emulate_srgb::type const emulate_srgb_;
};
}

#endif
