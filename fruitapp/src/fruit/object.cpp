#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/mesh_to_point_cloud.hpp>
#include <fruitapp/fruit/mesh_to_shape.hpp>
#include <fruitapp/fruit/mesh_to_vertex_buffer.hpp>
#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitlib/def_ctor.hpp>
#include <fruitlib/geometry_traits/box.hpp>
#include <fruitlib/geometry_traits/vector.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/group/sequence.hpp>
#include <fruitlib/physics/rigid_body/parameters.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vertex/buffer.hpp>
#include <sge/timer/parameters.hpp>
#include <fcppt/make_ref.hpp>
#include <fcppt/cast/size_fun.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::object::object(
	fruitapp::fruit::prototype const &_prototype,
	fruitlib::physics::world &_physics_world,
	sge::renderer::device::core &_renderer,
	sge::renderer::vertex::declaration &_vertex_declaration,
	fruitapp::fruit::mesh_unique_ptr _mesh,
	fruitlib::physics::group::object &_fruit_group,
	fruitlib::physics::rigid_body::mass const &_mass,
	fruitlib::physics::rigid_body::position const &_position,
	fruitlib::physics::rigid_body::transformation const &_transformation,
	fruitlib::physics::rigid_body::linear_velocity const &_linear_velocity,
	fruitlib::physics::rigid_body::angular_velocity const &_angular_velocity,
	fruitapp::fruit::ban_duration const &_lock_duration,
	fruitapp::ingame_clock const &_clock)
:
	prototype_(
		_prototype),
	mesh_(
		std::move(
			_mesh)),
	bounding_box_(
		boost::geometry::return_envelope<
			fruitlib::def_ctor<
				fruitapp::fruit::box3
			>
		>(
			fruitapp::fruit::mesh_to_point_cloud(
				*mesh_
			)
		).base()
	),
	body_(
		fruitlib::physics::rigid_body::parameters(
			_position,
			_transformation,
			_linear_velocity,
			_angular_velocity,
			fruitapp::fruit::mesh_to_shape(
				*mesh_),
			fruitlib::physics::rigid_body::solidity::solid,
			fruitlib::physics::rigid_body::optional_mass(
				_mass),
			fruitlib::physics::rigid_body::user_data())),
	body_scope_(
		_physics_world,
		body_,
		fruitlib::physics::group::sequence{
			fcppt::make_ref(
				_fruit_group)}),
	vb_(
		fruitapp::fruit::mesh_to_vertex_buffer(
			_renderer,
			_vertex_declaration,
			*mesh_)),
	// A fruit originating from another fruit is banned for a specific duration
	lock_timer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			_lock_duration.get()))
{
}

sge::renderer::vertex::buffer &
fruitapp::fruit::object::vb()
{
	return *vb_;
}

sge::renderer::vertex::buffer const &
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
		fcppt::math::matrix::structure_cast<sge::renderer::matrix4, fcppt::cast::size_fun>(
			body_.world_transform().get());
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
		fcppt::math::matrix::structure_cast<sge::renderer::matrix4, fcppt::cast::size_fun>(
			body_.transformation().get());
}

sge::renderer::vector3 const
fruitapp::fruit::object::position() const
{
	return
		fcppt::math::vector::structure_cast<sge::renderer::vector3, fcppt::cast::size_fun>(
			body_.position().get());
}

fruitapp::fruit::box3 const &
fruitapp::fruit::object::bounding_box() const
{
	return bounding_box_;
}

fruitapp::fruit::mesh const &
fruitapp::fruit::object::mesh() const
{
	return *mesh_;
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
