#include "object.hpp"
#include "prototype.hpp"
#include "../mesh_to_vertex_buffer.hpp"
#include "../mesh_to_shape.hpp"
#include "../geometry_traits/box.hpp"
#include "../geometry_traits/vector.hpp"
#include "../physics/rigid_body/parameters.hpp"
#include "../physics/world_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/box/output.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/foreach.hpp>
#include <vector>

#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/dim.hpp>
#include <fcppt/math/vector/arithmetic.hpp>

namespace
{
typedef
boost::geometry::model::multi_point<fruitcut::app::triangle::vector>
point_cloud;

point_cloud const
mesh_to_point_cloud(
	fruitcut::app::mesh const &m)
{
	point_cloud output;
	output.reserve(
		static_cast<point_cloud::size_type>(
			m.triangles.size() * 3));

	BOOST_FOREACH(
		fruitcut::app::triangle const &t,
		m.triangles)
		BOOST_FOREACH(
			fruitcut::app::triangle::vertex_array::const_reference r,
			t.vertices)
			output.push_back(
				r);

	return output;
}
}

fruitcut::app::fruit::object::object(
	prototype const &proto,
	physics::world &_world,
	sge::renderer::device_ptr const _renderer,
	sge::shader::object &_shader,
	physics::scalar const _mass,
	physics::vector3 const &_position,
	physics::matrix4 const &_transformation,
	physics::vector3 const &_linear_velocity,
	physics::vector3 const &_angular_velocity)
:
	mesh_(
		proto.mesh()),
	bounding_box_(
		boost::geometry::make_envelope<box3>(
			mesh_to_point_cloud(
				mesh_))),
	body_(
		physics::rigid_body::parameters(
			_world,
			_position,
			_transformation,
			_linear_velocity,
			_angular_velocity,
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
		proto.texture()),
	// A fruit obtained directly from a prototype shouldn't be locked,
	// so set the lock time to "zero"
	lock_timer_(
		sge::time::duration(),
		sge::time::activation_state::inactive)
{
}

fruitcut::app::fruit::object::object(
	sge::renderer::texture::planar_ptr const _texture,
	physics::world &_world,
	sge::renderer::device_ptr const _renderer,
	sge::shader::object &_shader,
	fruitcut::app::mesh const &_mesh,
	physics::scalar const _mass,
	physics::vector3 const &_position,
	physics::matrix4 const &_transformation,
	physics::vector3 const &_linear_velocity,
	physics::vector3 const &_angular_velocity,
	sge::time::duration const &_lock_duration,
	sge::time::callback const &_timer_callback)
:
	mesh_(
		_mesh),
	bounding_box_(
		boost::geometry::make_envelope<box3>(
			mesh_to_point_cloud(
				mesh_))),
	body_(
		physics::rigid_body::parameters(
			_world,
			_position,
			_transformation,
			_linear_velocity,
			_angular_velocity,
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
		_texture),
	// A fruit originating from another fruit is banned for a specific duration
	lock_timer_(
		_lock_duration,
		sge::time::activation_state::active,
		_timer_callback)
{
}

sge::renderer::vertex_buffer_ptr const
fruitcut::app::fruit::object::vb() const
{
	return vb_;
}

sge::renderer::texture::planar_ptr const
fruitcut::app::fruit::object::texture() const
{
	return texture_;
}

sge::renderer::matrix4 const
fruitcut::app::fruit::object::world_transform() const
{
	return 
		fcppt::math::matrix::structure_cast<sge::renderer::matrix4>(
			body_.world_transform());
}

fruitcut::physics::rigid_body::object const &
fruitcut::app::fruit::object::body() const
{
	return 
		body_;
}

sge::renderer::matrix4 const
fruitcut::app::fruit::object::rotation() const
{
	return 
		fcppt::math::matrix::structure_cast<sge::renderer::matrix4>(
			body_.rotation());
}

sge::renderer::vector3 const
fruitcut::app::fruit::object::position() const
{
	return 
		fcppt::math::vector::structure_cast<sge::renderer::vector3>(
			body_.position());
}

fruitcut::app::box3 const &
fruitcut::app::fruit::object::bounding_box() const
{
	return bounding_box_;
}

fruitcut::app::mesh const &
fruitcut::app::fruit::object::mesh() const
{
	return mesh_;
}

bool
fruitcut::app::fruit::object::locked() const
{
	// Either we weren't locked in the first place or we were locked and
	// are not locked anymore
	return lock_timer_.active() && !lock_timer_.expired();
}

fruitcut::app::fruit::object::~object()
{
}
