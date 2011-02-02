#ifndef FRUITCUT_APP_FRUIT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_HPP_INCLUDED

#include "mesh.hpp"
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
#include <sge/shader/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
class fruit
{
FCPPT_NONCOPYABLE(
	fruit)
public:
	explicit
	fruit(
		fruit_prototype const &,
		physics::world &,
		sge::renderer::device_ptr,
		sge::shader::object &,
		sge::renderer::texture_ptr,
		physics::scalar mass,
		physics::vector3 const &position,
		physics::matrix4 const &transformation,
		physics::vector3 const &linear_velocity);

	~fruit();
private:
	mesh mesh_;
	physics::rigid_body::object body_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::texture_ptr texture_;
};
}
}

#endif
