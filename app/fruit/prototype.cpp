#include "prototype.hpp"
#include "mesh_to_point_cloud.hpp"
#include "../../geometry_traits/box.hpp"
#include <boost/geometry/geometry.hpp>

fruitcut::app::fruit::prototype::prototype(
	fruitcut::app::fruit::mesh const &_mesh,
	sge::renderer::texture::planar_ptr const _texture)
:
	mesh_(
		_mesh),
	bounding_box_(
		boost::geometry::make_envelope<box3>(
			mesh_to_point_cloud(
				mesh_))),
	texture_(
		_texture)
	
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
