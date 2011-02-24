#ifndef FRUITCUT_APP_FRUIT_OBJECT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_OBJECT_HPP_INCLUDED

#include "../mesh.hpp"
#include "../box3.hpp"
#include "prototype_fwd.hpp"
#include "../../physics/rigid_body/object.hpp"
#include "../../physics/world_fwd.hpp"
#include "../../physics/scalar.hpp"
#include "../../physics/vector3.hpp"
#include "../../physics/matrix4.hpp"
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/shader/object_fwd.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/duration.hpp>
#include <sge/time/callback.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/chrono/time_point.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
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
		prototype const &,
		physics::world &,
		sge::renderer::device_ptr,
		sge::shader::object &,
		physics::scalar mass,
		physics::vector3 const &position,
		physics::matrix4 const &transformation,
		physics::vector3 const &linear_velocity,
		physics::vector3 const &angular_velocity);

	// Create from other fruit
	explicit
	object(
		sge::renderer::texture::planar_ptr,
		physics::world &,
		sge::renderer::device_ptr,
		sge::shader::object &,
		fruitcut::app::mesh const &,
		physics::scalar mass,
		physics::vector3 const &position,
		physics::matrix4 const &transformation,
		physics::vector3 const &linear_velocity,
		physics::vector3 const &angular_velocity,
		sge::time::duration const &,
		sge::time::callback const &);

	sge::renderer::vertex_buffer_ptr const
	vb() const;

	sge::renderer::texture::planar_ptr const
	texture() const;

	sge::renderer::matrix4 const
	world_transform() const;

	physics::rigid_body::object const &
	body() const;

	sge::renderer::matrix4 const 
	rotation() const;

	sge::renderer::vector3 const 
	position() const;

	box3 const &
	bounding_box() const;

	fruitcut::app::mesh const &
	mesh() const;

	bool
	locked() const;

	~object();
private:
	fruitcut::app::mesh mesh_;
	box3 bounding_box_;
	physics::rigid_body::object body_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::texture::planar_ptr texture_;
	sge::time::timer lock_timer_;
};
}
}
}

#endif
