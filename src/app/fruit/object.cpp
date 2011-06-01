#include "object.hpp"
#include "mesh_to_vertex_buffer.hpp"
#include "mesh_to_point_cloud.hpp"
#include "object_parameters.hpp"
#include "mesh_to_shape.hpp"
#include "box3.hpp"
#include "../../fruitlib/geometry_traits/box.hpp"
#include "../../fruitlib/geometry_traits/vector.hpp"
#include "../../fruitlib/physics/rigid_body/parameters.hpp"
#include "../../fruitlib/physics/world_fwd.hpp"
#include <sge/time/activation_state.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <boost/geometry/geometry.hpp>

fruitcut::app::fruit::object::object(
	object_parameters const &p)
:
	mesh_(
		p.mesh()),
	bounding_box_(
		boost::geometry::return_envelope<box3>(
			fruit::mesh_to_point_cloud(
				mesh_))),
	splatter_color_(
		p.splatter_color()),
	body_(
		fruitlib::physics::rigid_body::parameters(
			p.physics_world(),
			p.position(),
			p.transformation(),
			p.linear_velocity(),
			p.angular_velocity(),
			fruit::mesh_to_shape(
				mesh_),
			fruitlib::physics::rigid_body::solidity::solid,
			p.mass())),
	vb_(
		fruit::mesh_to_vertex_buffer(
			p.renderer(),
			p.vertex_declaration(),
			p.shader(),
			mesh_)),
	texture_(
		p.texture()),
	// A fruit originating from another fruit is banned for a specific duration
	lock_timer_(
		p.lock_duration(),
		sge::time::activation_state::active,
		p.timer_callback())
{
}

sge::renderer::vertex_buffer &
fruitcut::app::fruit::object::vb()
{
	return *vb_;
}

sge::renderer::vertex_buffer const &
fruitcut::app::fruit::object::vb() const
{
	return *vb_;
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

fruitcut::fruitlib::physics::rigid_body::object const &
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
			body_.transformation());
}

sge::renderer::vector3 const
fruitcut::app::fruit::object::position() const
{
	return 
		fcppt::math::vector::structure_cast<sge::renderer::vector3>(
			body_.position());
}

fruitcut::app::fruit::box3 const &
fruitcut::app::fruit::object::bounding_box() const
{
	return bounding_box_;
}

sge::image::color::any::object const &
fruitcut::app::fruit::object::splatter_color() const
{
	return splatter_color_;
}

fruitcut::app::fruit::mesh const &
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
