#include <fruitapp/fruit/prototype.hpp>
#include <fruitapp/fruit/mesh_to_point_cloud.hpp>
#include <fruitlib/geometry_traits/box.hpp>
#include <fruitlib/average_colors.hpp>
#include <sge/image2d/view/sub.hpp>
#include <sge/image2d/rect.hpp>
#include <sge/image2d/dim.hpp>
#include <sge/image/colors.hpp>
#include <sge/image2d/view/to_const.hpp>
#include <sge/renderer/texture/scoped_planar_lock.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/box/box.hpp>
#include <boost/geometry/geometry.hpp>

fruitapp::fruit::prototype::prototype(
	fruitapp::fruit::mesh const &_mesh,
	sge::renderer::texture::planar_ptr const _texture,
	material::object const &_material,
	fruit::tag_set const &_tags)
:
	mesh_(
		_mesh),
	bounding_box_(
		boost::geometry::return_envelope<box3>(
			mesh_to_point_cloud(
				mesh_))),
	mass_(
		bounding_box_.content()),
	texture_(
		_texture),
	splatter_color_(
		fruitlib::average_colors(
			sge::image2d::view::to_const(
				sge::image2d::view::sub(
					sge::renderer::texture::scoped_planar_lock(
						*_texture,
						sge::renderer::lock_mode::readwrite).value(),
					sge::image2d::rect(
						sge::image2d::rect::vector(
							_texture->size().w()/2,
							0),
						sge::image2d::dim(
							_texture->size().w()/2,
							_texture->size().h())))))),
	material_(
		_material),
	tags_(
		_tags)
{
}

fruitapp::fruit::mesh const &
fruitapp::fruit::prototype::mesh() const
{
	return mesh_;
}

fruitapp::fruit::box3 const &
fruitapp::fruit::prototype::bounding_box() const
{
	return bounding_box_;
}

fruitlib::physics::rigid_body::mass::value_type
fruitapp::fruit::prototype::mass() const
{
	return mass_;
}

sge::renderer::texture::planar_ptr const 
fruitapp::fruit::prototype::texture() const
{
	return texture_;
}

sge::image::color::any::object const &
fruitapp::fruit::prototype::splatter_color() const
{
	return splatter_color_;
}

fruitapp::fruit::material::object const &
fruitapp::fruit::prototype::material() const
{
	return material_;
}

fruitapp::fruit::tag_set const &
fruitapp::fruit::prototype::tags() const
{
	return tags_;
}
