#ifndef FRUITCUT_APP_FRUIT_OBJECT_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_OBJECT_PARAMETERS_HPP_INCLUDED

#include "mesh.hpp"
#include "../../fruitlib/physics/scalar.hpp"
#include "../../fruitlib/physics/world_fwd.hpp"
#include "../../fruitlib/physics/vector3.hpp"
#include "../../fruitlib/physics/matrix4.hpp"
#include "../../fruitlib/physics/group/object_fwd.hpp"
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/shader/object_fwd.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/variant/object.hpp>

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
		sge::image::color::any::object const _splatter_color,
		fruitlib::physics::world &_world,
		sge::renderer::device &_renderer,
		sge::renderer::vertex_declaration &_vertex_declaration,
		sge::shader::object &_shader,
		fruitcut::app::fruit::mesh const &_mesh,
		fruitlib::physics::group::object &_fruit_group,
		fruitlib::physics::scalar const _mass,
		fruitlib::physics::vector3 const &_position,
		fruitlib::physics::matrix4 const &_transformation,
		fruitlib::physics::vector3 const &_linear_velocity,
		fruitlib::physics::vector3 const &_angular_velocity,
		sge::time::duration const &_lock_duration,
		sge::time::callback const &_timer_callback);

	sge::renderer::texture::planar_ptr const 
	texture() const;

	sge::image::color::any::object const &
	splatter_color() const;

	fruitlib::physics::world &
	physics_world() const;

	sge::renderer::device &
	renderer() const;

	sge::renderer::vertex_declaration &
	vertex_declaration() const;

	sge::shader::object &
	shader() const;

	fruitcut::app::fruit::mesh const &
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

	sge::time::duration const &
	lock_duration() const;

	sge::time::callback const &
	timer_callback() const;
private:
	sge::renderer::texture::planar_ptr const texture_;
	sge::image::color::any::object const splatter_color_;
	fruitlib::physics::world &world_;
	sge::renderer::device &renderer_;
	sge::renderer::vertex_declaration &vertex_declaration_;
	sge::shader::object &shader_;
	fruitcut::app::fruit::mesh const mesh_;
	fruitlib::physics::group::object &fruit_group_;
	fruitlib::physics::scalar const mass_;
	fruitlib::physics::vector3 const position_;
	fruitlib::physics::matrix4 const transformation_;
	fruitlib::physics::vector3 const linear_velocity_;
	fruitlib::physics::vector3 const angular_velocity_;
	sge::time::duration const lock_duration_;
	sge::time::callback const timer_callback_;
};
}
}
}

#endif
