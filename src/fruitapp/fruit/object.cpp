#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/mesh_to_point_cloud.hpp>
#include <fruitapp/fruit/mesh_to_shape.hpp>
#include <fruitapp/fruit/mesh_to_vertex_buffer.hpp>
#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/object_parameters.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitlib/geometry_traits/box.hpp>
#include <fruitlib/geometry_traits/vector.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/group/sequence.hpp>
#include <fruitlib/physics/rigid_body/parameters.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::object::object(
	object_parameters const &p)
:
	prototype_(
		p.prototype()),
	mesh_(
		p.mesh()),
	bounding_box_(
		boost::geometry::return_envelope<box3>(
			fruit::mesh_to_point_cloud(
				mesh_))),
	body_(
		fruitlib::physics::rigid_body::parameters(
			fruitlib::physics::rigid_body::position(
				p.position()),
			fruitlib::physics::rigid_body::transformation(
				p.transformation()),
			fruitlib::physics::rigid_body::linear_velocity(
				p.linear_velocity()),
			fruitlib::physics::rigid_body::angular_velocity(
				p.angular_velocity()),
			fruit::mesh_to_shape(
				mesh_),
			fruitlib::physics::rigid_body::solidity::solid,
			fruitlib::physics::rigid_body::optional_mass(
				fruitlib::physics::rigid_body::mass(
					p.mass())),
			fruitlib::physics::rigid_body::user_data())),
	body_scope_(
		p.physics_world(),
		body_,
		fcppt::assign::make_container<fruitlib::physics::group::sequence>(
			fcppt::ref(
				p.fruit_group()))),
	vb_(
		fruit::mesh_to_vertex_buffer(
			p.renderer(),
			p.vertex_declaration(),
			mesh_)),
	// A fruit originating from another fruit is banned for a specific duration
	lock_timer_(
		fruitapp::ingame_timer::parameters(
			p.clock(),
			p.lock_duration()))
{
}

sge::renderer::vertex_buffer &
fruitapp::fruit::object::vb()
{
	return *vb_;
}

sge::renderer::vertex_buffer const &
fruitapp::fruit::object::vb() const
{
	return *vb_;
}

fruitapp::fruit::prototype const &
fruitapp::fruit::object::prototype() const
{
	return prototype_;
}

sge::renderer::matrix4 const
fruitapp::fruit::object::world_transform() const
{
	return
		fcppt::math::matrix::structure_cast<sge::renderer::matrix4>(
			body_.world_transform());
}

fruitlib::physics::rigid_body::object const &
fruitapp::fruit::object::body() const
{
	return
		body_;
}

sge::renderer::matrix4 const
fruitapp::fruit::object::rotation() const
{
	return
		fcppt::math::matrix::structure_cast<sge::renderer::matrix4>(
			body_.transformation());
}

sge::renderer::vector3 const
fruitapp::fruit::object::position() const
{
	return
		fcppt::math::vector::structure_cast<sge::renderer::vector3>(
			body_.position());
}

fruitapp::fruit::box3 const &
fruitapp::fruit::object::bounding_box() const
{
	return bounding_box_;
}

fruitapp::fruit::mesh const &
fruitapp::fruit::object::mesh() const
{
	return mesh_;
}

bool
fruitapp::fruit::object::locked() const
{
	// Either we weren't locked in the first place or we were locked and
	// are not locked anymore
	return lock_timer_.active() && !lock_timer_.expired();
}

fruitapp::fruit::object::~object()
{
}
