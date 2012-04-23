#ifndef FRUITAPP_FRUIT_OBJECT_HPP_INCLUDED
#define FRUITAPP_FRUIT_OBJECT_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_scoped_ptr.hpp>
#include <fruitapp/fruit/mesh_unique_ptr.hpp>
#include <fruitapp/fruit/prototype_fwd.hpp>
#include <fruitapp/fruit/prototype_fwd.hpp>
#include <fruitlib/physics/matrix4.hpp>
#include <fruitlib/physics/scalar.hpp>
#include <fruitlib/physics/vector3.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <fruitlib/physics/group/object_fwd.hpp>
#include <fruitlib/physics/rigid_body/object.hpp>
#include <fruitlib/physics/rigid_body/scoped.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vertex_buffer_scoped_ptr.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/variant/object.hpp>
#include <fcppt/variant/object.hpp>


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
	explicit
	object(
		fruit::prototype const &,
		fruitlib::physics::world &_world,
		sge::renderer::device &_renderer,
		sge::renderer::vertex_declaration &_vertex_declaration,
		fruitapp::fruit::mesh_unique_ptr _mesh,
		fruitlib::physics::group::object &_fruit_group,
		fruitlib::physics::scalar const _mass,
		fruitlib::physics::vector3 const &_position,
		fruitlib::physics::matrix4 const &_transformation,
		fruitlib::physics::vector3 const &_linear_velocity,
		fruitlib::physics::vector3 const &_angular_velocity,
		fruitapp::ingame_clock::duration const &_lock_duration,
		fruitapp::ingame_clock const &);

	sge::renderer::vertex_buffer &
	vb();

	sge::renderer::vertex_buffer const &
	vb() const;

	fruit::prototype const &
	prototype() const;

	sge::renderer::matrix4 const
	world_transform() const;

	fruitlib::physics::rigid_body::object const &
	body() const;

	sge::renderer::matrix4 const
	rotation() const;

	sge::renderer::vector3 const
	position() const;

	fruit::box3 const &
	bounding_box() const;

	fruitapp::fruit::mesh const &
	mesh() const;

	bool
	locked() const;

	~object();
private:
	fruit::prototype const &prototype_;
	fruitapp::fruit::mesh_scoped_ptr mesh_;
	box3 bounding_box_;
	fruitlib::physics::rigid_body::object body_;
	fruitlib::physics::rigid_body::scoped body_scope_;
	sge::renderer::vertex_buffer_scoped_ptr vb_;
	fruitapp::ingame_timer lock_timer_;
};
}
}

#endif
