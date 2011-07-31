#ifndef FRUITAPP_FRUIT_OBJECT_HPP_INCLUDED
#define FRUITAPP_FRUIT_OBJECT_HPP_INCLUDED

#include "box3.hpp"
#include "mesh.hpp"
#include "prototype_fwd.hpp"
#include "object_parameters_fwd.hpp"
#include "../../fruitlib/physics/rigid_body/object.hpp"
#include "../../fruitlib/physics/rigid_body/scoped.hpp"
#include <sge/renderer/vertex_buffer_fwd.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/noncopyable.hpp>
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
		object_parameters const &);

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
	fruitapp::fruit::mesh mesh_;
	box3 bounding_box_;
	fruitlib::physics::rigid_body::object body_;
	fruitlib::physics::rigid_body::scoped body_scope_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::time::timer lock_timer_;
};
}
}

#endif
