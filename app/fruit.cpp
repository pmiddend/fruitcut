#include "fruit.hpp"
#include "fruit_prototype.hpp"
#include "mesh_to_vertex_buffer.hpp"
#include "mesh_to_shape.hpp"
#include "../physics/rigid_body/parameters.hpp"
#include "../physics/world_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/shader/object_fwd.hpp>

fruitcut::app::fruit::fruit(
	fruit_prototype const &proto,
	physics::world &_world,
	sge::renderer::device_ptr const _renderer,
	sge::shader::object &_shader,
	physics::scalar const _mass,
	physics::vector3 const &_position,
	physics::matrix4 const &_transformation,
	physics::vector3 const &_linear_velocity)
:
	mesh_(
		proto.mesh()),
	body_(
		physics::rigid_body::parameters(
			_world,
			_position,
			_transformation,
			_linear_velocity,
			mesh_to_shape(
				mesh_),
			physics::rigid_body::solidity::solid,
			_mass)),
	vb_(
		mesh_to_vertex_buffer(
			_renderer,
			_shader,
			mesh_)),
	texture_(
		proto.texture())
{
}

sge::renderer::vertex_buffer_ptr const
fruitcut::app::fruit::vb() const
{
	return vb_;
}

sge::renderer::texture_ptr const
fruitcut::app::fruit::texture() const
{
	return texture_;
}

sge::renderer::matrix4 const
fruitcut::app::fruit::world_transform() const
{
	return body_.world_transform();
}

fruitcut::app::fruit::~fruit()
{
}
