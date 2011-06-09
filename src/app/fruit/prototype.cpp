#include "prototype.hpp"
#include "mesh_to_point_cloud.hpp"
#include "../../fruitlib/geometry_traits/box.hpp"
#include "../../fruitlib/average_colors.hpp"
#include <sge/image2d/view/sub.hpp>
#include <sge/image2d/rect.hpp>
#include <sge/image2d/dim.hpp>
#include <sge/image2d/view/to_const.hpp>
#include <sge/renderer/texture/scoped_planar_lock.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/box/box.hpp>
#include <boost/geometry/geometry.hpp>

fruitcut::app::fruit::prototype::prototype(
	fruitcut::app::fruit::mesh const &_mesh,
	sge::renderer::texture::planar_ptr const _texture,
	material::object const &_material)
:
	mesh_(
		_mesh),
	bounding_box_(
		boost::geometry::return_envelope<box3>(
			mesh_to_point_cloud(
				mesh_))),
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
		_material)
{
}

fruitcut::app::fruit::mesh const &
fruitcut::app::fruit::prototype::mesh() const
{
	return mesh_;
}

fruitcut::app::fruit::box3 const &
fruitcut::app::fruit::prototype::bounding_box() const
{
	return bounding_box_;
}

sge::renderer::texture::planar_ptr const 
fruitcut::app::fruit::prototype::texture() const
{
	return texture_;
}

sge::image::color::any::object const &
fruitcut::app::fruit::prototype::splatter_color() const
{
	return splatter_color_;
}

fruitcut::app::fruit::material::object const &
fruitcut::app::fruit::prototype::material() const
{
	return material_;
}
