#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_to_point_cloud.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitlib/geometry_traits/box.hpp>
#include <sge/image/color/any/object_fwd.hpp>
#include <sge/image2d/dim.hpp>
#include <sge/image2d/rect.hpp>
#include <sge/image2d/view/sub.hpp>
#include <sge/image2d/view/to_const.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/scoped_planar_lock.hpp>
#include <fcppt/math/dim/contents.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::prototype::prototype(
	fruitapp::fruit::mesh_unique_ptr _mesh,
	sge::renderer::texture::planar_shared_ptr const _texture,
	fruitapp::fruit::material::object const &_material,
	fruitapp::fruit::splatter_color const &_splatter_color,
	fruitapp::fruit::tag_set const &_tags)
:
	mesh_(
		std::move(
			_mesh)),
	bounding_box_(
		boost::geometry::return_envelope<fruitapp::fruit::box3>(
			mesh_to_point_cloud(
				*mesh_))),
	mass_(
		fcppt::math::dim::contents(
			bounding_box_.size()
		)
	),
	texture_(
		_texture),
	splatter_color_(
		_splatter_color.get()),
	material_(
		_material),
	tags_(
		_tags)
{
}

fruitapp::fruit::mesh const &
fruitapp::fruit::prototype::mesh() const
{
	return
		*mesh_;
}

fruitapp::fruit::box3 const &
fruitapp::fruit::prototype::bounding_box() const
{
	return
		bounding_box_;
}

fruitlib::physics::rigid_body::mass const
fruitapp::fruit::prototype::mass() const
{
	return
		mass_;
}

sge::renderer::texture::planar_shared_ptr const
fruitapp::fruit::prototype::texture() const
{
	return
		texture_;
}

sge::image::color::any::object const &
fruitapp::fruit::prototype::splatter_color() const
{
	return
		splatter_color_;
}

fruitapp::fruit::material::object const &
fruitapp::fruit::prototype::material() const
{
	return
		material_;
}

fruitapp::fruit::tag_set const &
fruitapp::fruit::prototype::tags() const
{
	return
		tags_;
}

fruitapp::fruit::prototype::~prototype()
{
}
