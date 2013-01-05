#ifndef FRUITAPP_FRUIT_OBJECT_FROM_PROTOTYPE_HPP_INCLUDED
#define FRUITAPP_FRUIT_OBJECT_FROM_PROTOTYPE_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/fruit/object_unique_ptr.hpp>
#include <fruitapp/fruit/prototype_fwd.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/group/object_fwd.hpp>
#include <fruitlib/physics/rigid_body/angular_velocity.hpp>
#include <fruitlib/physics/rigid_body/linear_velocity.hpp>
#include <fruitlib/physics/rigid_body/mass.hpp>
#include <fruitlib/physics/rigid_body/position.hpp>
#include <fruitlib/physics/rigid_body/transformation.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/renderer/vertex/declaration_fwd.hpp>


namespace fruitapp
{
namespace fruit
{
fruitapp::fruit::object_unique_ptr
object_from_prototype(
	fruitapp::fruit::prototype const &proto,
	fruitlib::physics::world &_world,
	fruitlib::physics::group::object &_fruit_group,
	sge::renderer::device::core &_renderer,
	sge::renderer::vertex::declaration &_vertex_declaration,
	fruitlib::physics::rigid_body::mass const _mass,
	fruitlib::physics::rigid_body::position const &_position,
	fruitlib::physics::rigid_body::transformation const &_transformation,
	fruitlib::physics::rigid_body::linear_velocity const &_linear_velocity,
	fruitlib::physics::rigid_body::angular_velocity const &_angular_velocity,
	fruitapp::ingame_clock const &);
}
}

#endif

