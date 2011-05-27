#ifndef FRUITCUT_APP_FRUIT_OBJECT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_OBJECT_HPP_INCLUDED

#include "box3.hpp"
#include "mesh.hpp"
#include "object_parameters_fwd.hpp"
#include "../../physics/rigid_body/object.hpp"
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
		object_parameters const &);

	sge::renderer::vertex_buffer &
	vb();

	sge::renderer::vertex_buffer const &
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

	fruit::box3 const &
	bounding_box() const;

	fruitcut::app::fruit::mesh const &
	mesh() const;

	bool
	locked() const;

	~object();
private:
	fruitcut::app::fruit::mesh mesh_;
	box3 bounding_box_;
	//sge::image::color::any::object splatter_color_;
	physics::rigid_body::object body_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::texture::planar_ptr texture_;
	sge::time::timer lock_timer_;
};
}
}
}

#endif
