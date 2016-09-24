#ifndef FRUITAPP_BACKGROUND_CG_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_CG_HPP_INCLUDED

#include <fruitapp/background/base.hpp>
#include <fruitapp/background/repetitions.hpp>
#include <fruitapp/shadow_map/optional_object_ref.hpp>
#include <fruitlib/texture_manager_fwd.hpp>
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
		fruitlib::texture_manager &_texture_manager,
		sge::shader::context &,
		fruitapp::shadow_map::optional_object_ref const &,
		fruitapp::background::repetitions const &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &);

	void
	render(
		sge::renderer::context::core &
	)
	override;

	~cg()
	override;
private:
	sge::camera::base const &camera_;
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
