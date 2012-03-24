#ifndef FRUITAPP_FRUIT_OBJECT_PARAMETERS_HPP_INCLUDED
#define FRUITAPP_FRUIT_OBJECT_PARAMETERS_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/prototype_fwd.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/group/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/variant/object.hpp>


namespace fruitapp
{
namespace fruit
{
class object_parameters
{
FCPPT_NONASSIGNABLE(
	object_parameters);
public:
	explicit
	object_parameters(
		fruit::prototype const &,
		fruitlib::physics::world &_world,
		sge::renderer::device &_renderer,
		sge::renderer::vertex_declaration &_vertex_declaration,
		fruitapp::fruit::mesh const &_mesh,
		fruitlib::physics::group::object &_fruit_group,
		fruitlib::physics::scalar const _mass,
		fruitlib::physics::vector3 const &_position,
		fruitlib::physics::matrix4 const &_transformation,
		fruitlib::physics::vector3 const &_linear_velocity,
		fruitlib::physics::vector3 const &_angular_velocity,
		fruitapp::ingame_clock::duration const &_lock_duration,
		fruitapp::ingame_clock const &);

	fruit::prototype const &
	prototype() const;

	fruitlib::physics::world &
	physics_world() const;

	sge::renderer::device &
	renderer() const;

	sge::renderer::vertex_declaration &
	vertex_declaration() const;

	fruitapp::fruit::mesh const &
	mesh() const;

	fruitlib::physics::group::object &
	fruit_group() const;

	fruitlib::physics::scalar
	mass() const;

	fruitlib::physics::vector3 const &
	position() const;

	fruitlib::physics::matrix4 const &
	transformation() const;

	fruitlib::physics::vector3 const &
	linear_velocity() const;

	fruitlib::physics::vector3 const &
	angular_velocity() const;

	fruitapp::ingame_clock::duration const &
	lock_duration() const;

	fruitapp::ingame_clock const &
	clock() const;
private:
	fruit::prototype const &prototype_;
	fruitlib::physics::world &world_;
	sge::renderer::device &renderer_;
	sge::renderer::vertex_declaration &vertex_declaration_;
	fruitapp::fruit::mesh const mesh_;
	fruitlib::physics::group::object &fruit_group_;
	fruitlib::physics::scalar const mass_;
	fruitlib::physics::vector3 const position_;
	fruitlib::physics::matrix4 const transformation_;
	fruitlib::physics::vector3 const linear_velocity_;
	fruitlib::physics::vector3 const angular_velocity_;
	fruitapp::ingame_clock::duration const lock_duration_;
	fruitapp::ingame_clock const &clock_;
};
}
}

#endif
