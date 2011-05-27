#ifndef FRUITCUT_APP_FRUIT_OBJECT_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_OBJECT_PARAMETERS_HPP_INCLUDED

#include "mesh.hpp"
#include "../../physics/scalar.hpp"
#include "../../physics/world_fwd.hpp"
#include "../../physics/vector3.hpp"
#include "../../physics/matrix4.hpp"
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/shader/object_fwd.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/nonassignable.hpp>

namespace fruitcut
{
namespace app
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
		sge::renderer::texture::planar_ptr const _texture,
		physics::world &_world,
		sge::renderer::device &_renderer,
		sge::renderer::vertex_declaration &_vertex_declaration,
		sge::shader::object &_shader,
		fruitcut::app::fruit::mesh const &_mesh,
		physics::scalar const _mass,
		physics::vector3 const &_position,
		physics::matrix4 const &_transformation,
		physics::vector3 const &_linear_velocity,
		physics::vector3 const &_angular_velocity,
		sge::time::duration const &_lock_duration,
		sge::time::callback const &_timer_callback);

	sge::renderer::texture::planar_ptr const 
	texture() const;

	physics::world &
	physics_world() const;

	sge::renderer::device &
	renderer() const;

	sge::renderer::vertex_declaration &
	vertex_declaration() const;

	sge::shader::object &
	shader() const;

	fruitcut::app::fruit::mesh const &
	mesh() const;

	physics::scalar 
	mass() const;

	physics::vector3 const &
	position() const;

	physics::matrix4 const &
	transformation() const;

	physics::vector3 const &
	linear_velocity() const;

	physics::vector3 const &
	angular_velocity() const;

	sge::time::duration const &
	lock_duration() const;

	sge::time::callback const &
	timer_callback() const;
private:
	sge::renderer::texture::planar_ptr const texture_;
	physics::world &world_;
	sge::renderer::device &renderer_;
	sge::renderer::vertex_declaration &vertex_declaration_;
	sge::shader::object &shader_;
	fruitcut::app::fruit::mesh const mesh_;
	physics::scalar const mass_;
	physics::vector3 const position_;
	physics::matrix4 const transformation_;
	physics::vector3 const linear_velocity_;
	physics::vector3 const angular_velocity_;
	sge::time::duration const lock_duration_;
	sge::time::callback const timer_callback_;
};
}
}
}

#endif
