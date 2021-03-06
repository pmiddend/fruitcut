#ifndef FRUITAPP_FRUIT_OBJECT_HPP_INCLUDED
#define FRUITAPP_FRUIT_OBJECT_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/fruit/ban_duration.hpp>
#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_unique_ptr.hpp>
#include <fruitapp/fruit/prototype_fwd.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/group/object_fwd.hpp>
#include <fruitlib/physics/rigid_body/angular_velocity.hpp>
#include <fruitlib/physics/rigid_body/linear_velocity.hpp>
#include <fruitlib/physics/rigid_body/mass.hpp>
#include <fruitlib/physics/rigid_body/object.hpp>
#include <fruitlib/physics/rigid_body/position.hpp>
#include <fruitlib/physics/rigid_body/scoped.hpp>
#include <fruitlib/physics/rigid_body/transformation.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/renderer/vertex/buffer_fwd.hpp>
#include <sge/renderer/vertex/buffer_unique_ptr.hpp>
#include <sge/renderer/vertex/declaration_fwd.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace fruit
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	// Create from prototype
	object(
		fruitapp::fruit::prototype const &,
		fruitlib::physics::world &_world,
		sge::renderer::device::core &_renderer,
		sge::renderer::vertex::declaration &_vertex_declaration,
		fruitapp::fruit::mesh_unique_ptr _mesh,
		fruitlib::physics::group::object &_fruit_group,
		fruitlib::physics::rigid_body::mass const &_mass,
		fruitlib::physics::rigid_body::position const &_position,
		fruitlib::physics::rigid_body::transformation const &_transformation,
		fruitlib::physics::rigid_body::linear_velocity const &_linear_velocity,
		fruitlib::physics::rigid_body::angular_velocity const &_angular_velocity,
		fruitapp::fruit::ban_duration const &,
		fruitapp::ingame_clock const &);

	sge::renderer::vertex::buffer &
	vb();

	sge::renderer::vertex::buffer const &
	vb() const;

	fruitapp::fruit::prototype const &
	prototype() const;

	sge::renderer::matrix4 const
	world_transform() const;

	fruitlib::physics::rigid_body::object const &
	body() const;

	sge::renderer::matrix4 const
	rotation() const;

	sge::renderer::vector3 const
	position() const;

	fruitapp::fruit::box3 const &
	bounding_box() const;

	fruitapp::fruit::mesh const &
	mesh() const;

	bool
	locked() const;

	~object();
private:
	fruit::prototype const &prototype_;
	fruitapp::fruit::mesh_unique_ptr const mesh_;
	fruitapp::fruit::box3 bounding_box_;
	fruitlib::physics::rigid_body::object body_;
	fruitlib::physics::rigid_body::scoped body_scope_;
	sge::renderer::vertex::buffer_unique_ptr const vb_;
	fruitapp::ingame_timer lock_timer_;
};
}
}

#endif
