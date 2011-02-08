#ifndef FRUITCUT_APP_FRUIT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_HPP_INCLUDED

#include "mesh.hpp"
#include "box3.hpp"
#include "fruit_prototype_fwd.hpp"
#include "../physics/rigid_body/object.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/scalar.hpp"
#include "../physics/vector3.hpp"
#include "../physics/matrix4.hpp"
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/shader/object_fwd.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
class fruit
{
FCPPT_NONCOPYABLE(
	fruit);
public:
	explicit
	fruit(
		fruit_prototype const &,
		physics::world &,
		sge::renderer::device_ptr,
		sge::shader::object &,
		physics::scalar mass,
		physics::vector3 const &position,
		physics::matrix4 const &transformation,
		physics::vector3 const &linear_velocity);

	sge::renderer::vertex_buffer_ptr const
	vb() const;

	sge::renderer::texture_ptr const
	texture() const;

	sge::renderer::matrix4 const
	world_transform() const;

	box3 const &
	bounding_box() const;
	
	~fruit();
private:
	mesh mesh_;
	box3 bounding_box_;
	physics::rigid_body::object body_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::texture_ptr texture_;
};
}
}

#endif
