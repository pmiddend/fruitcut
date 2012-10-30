#ifndef FRUITAPP_FRUIT_RENDERING_CG_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_CG_HPP_INCLUDED

#include <fruitapp/ambient_intensity.hpp>
#include <fruitapp/directional_light_source_fwd.hpp>
#include <fruitapp/fruit/manager_fwd.hpp>
#include <fruitapp/fruit/rendering/base.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/state/core/depth_stencil/object_scoped_ptr.hpp>
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
class cg
:
	public fruitapp::fruit::rendering::base
{
FCPPT_NONCOPYABLE(
	cg);
public:
	cg(
		sge::shader::context &,
		fruitapp::fruit::manager const &,
		sge::camera::base const &,
		fruitapp::directional_light_source const &,
		fruitapp::ambient_intensity const &);

	void
	render(
		sge::renderer::context::core &);

	~cg();
private:
	fruitapp::fruit::manager const &manager_;
	sge::camera::base const &camera_;
	sge::shader::pair shader_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> mvp_parameter_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> mv_it_parameter_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> world_parameter_;
	sge::shader::parameter::vector<sge::renderer::scalar,3> light_position_parameter_;
	sge::shader::parameter::scalar<sge::renderer::scalar> ambient_intensity_parameter_;
	sge::shader::parameter::vector<sge::renderer::scalar,4> diffuse_color_parameter_;
	sge::shader::parameter::vector<sge::renderer::scalar,4> specular_color_parameter_;
	sge::shader::parameter::scalar<sge::renderer::scalar> diffuse_coefficient_parameter_;
	sge::shader::parameter::scalar<sge::renderer::scalar> specular_coefficient_parameter_;
	sge::shader::parameter::scalar<sge::renderer::scalar> specular_shininess_parameter_;
	sge::shader::parameter::planar_texture texture_parameter_;
	sge::renderer::state::core::depth_stencil::object_scoped_ptr const depth_stencil_state_;
};
}
}
}

#endif
