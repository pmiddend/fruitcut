#ifndef FRUITAPP_BACKGROUND_CG_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_CG_HPP_INCLUDED

#include <fruitapp/background/base.hpp>
#include <fruitapp/shadow_map/optional_object_ref.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/texture/planar_scoped_ptr.hpp>
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <sge/shader/context_fwd.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/matrix.hpp>
#include <sge/shader/parameter/planar_texture.hpp>
#include <sge/shader/parameter/scalar.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitapp
{
namespace background
{
class cg
:
	public fruitapp::background::base
{
FCPPT_NONCOPYABLE(
	cg);
public:
	cg(
		sge::image2d::system &,
		sge::shader::context &,
		fruitapp::shadow_map::optional_object_ref const &,
		sge::parse::json::object const &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &,
		sge::renderer::texture::emulate_srgb::type const &);

	void
	render(
		sge::renderer::context::core &);

	~cg();
private:
	sge::camera::base const &camera_;
	sge::renderer::texture::planar_scoped_ptr const texture_;
	sge::shader::pair shader_;
	sge::shader::parameter::scalar<sge::renderer::scalar> use_shadow_map_parameter_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> mvp_parameter_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> shadow_mvp_parameter_;
	sge::shader::parameter::planar_texture texture_parameter_;
	sge::shader::parameter::planar_texture shadow_map_parameter_;
};
}
}

#endif
