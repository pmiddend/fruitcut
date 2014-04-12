#ifndef FRUITAPP_FRUIT_RENDERING_FFP_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_FFP_HPP_INCLUDED

#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitapp/fruit/rendering/base.hpp>
#include <fruitapp/light/ambient_intensity.hpp>
#include <fruitapp/light/manager_fwd.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/state/core/depth_stencil/object_unique_ptr.hpp>
#include <sge/renderer/state/ffp/lighting/object_unique_ptr.hpp>
#include <sge/renderer/state/ffp/lighting/light/object_unique_ptr.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/matrix.hpp>
#include <sge/shader/parameter/planar_texture.hpp>
#include <sge/shader/parameter/scalar.hpp>
#include <sge/shader/parameter/vector.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace fruit
{
namespace rendering
{
class ffp
:
	public fruitapp::fruit::rendering::base
{
FCPPT_NONCOPYABLE(
	ffp);
public:
	ffp(
		sge::renderer::device::ffp &,
		fruitapp::fruit::manager const &,
		sge::camera::base const &,
		fruitapp::light::manager const &);

	void
	render(
		sge::renderer::context::core &);

	~ffp();
private:
	sge::renderer::device::ffp &renderer_;
	fruitapp::fruit::manager const &manager_;
	sge::camera::base const &camera_;
	fruitapp::light::ambient_intensity const ambient_intensity_;
	sge::renderer::state::core::depth_stencil::object_unique_ptr const depth_stencil_state_;
	sge::renderer::state::ffp::lighting::object_unique_ptr const lighting_;
	sge::renderer::state::ffp::lighting::light::object_unique_ptr const light_;
};
}
}
}

#endif
