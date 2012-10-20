#ifndef FRUITAPP_BACKGROUND_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_HPP_INCLUDED

#include <fruitapp/shadow_map_texture.hpp>
#include <fruitapp/shadow_mvp.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fruitlib/perspective_projection_information_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vertex_buffer_scoped_ptr.hpp>
#include <sge/renderer/vertex_declaration_scoped_ptr.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/renderer/state/core/depth_stencil/object_scoped_ptr.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/context_fwd.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/matrix.hpp>
#include <sge/shader/parameter/planar_texture.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class background
:
	public fruitlib::scenic::node<background>
{
FCPPT_NONCOPYABLE(
	background);
public:
	typedef
	boost::mpl::vector1
	<
		fruitlib::scenic::events::render
	>
	scene_reactions;

	background(
		fruitlib::scenic::optional_parent const &,
		sge::image2d::system &,
		sge::shader::context &,
		fruitapp::shadow_mvp const &,
		fruitapp::shadow_map_texture const &,
		sge::parse::json::object const &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &);

	~background();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	sge::camera::base const &camera_;
	sge::renderer::texture::planar_shared_ptr const texture_;
	sge::renderer::vertex_declaration_scoped_ptr const vertex_declaration_;
	sge::renderer::vertex_buffer_scoped_ptr const vb_;
	sge::shader::pair shader_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> mvp_parameter_;
	sge::shader::parameter::matrix<sge::renderer::scalar,4,4> shadow_mvp_parameter_;
	sge::shader::parameter::planar_texture texture_parameter_;
	sge::shader::parameter::planar_texture shadow_map_parameter_;
	sge::renderer::state::core::depth_stencil::object_scoped_ptr const depth_stencil_state_;
	sge::renderer::scalar const reps_;
	fcppt::signal::scoped_connection projection_change_connection_;

	void
	projection_change(
		fruitlib::perspective_projection_information const &);

};
}

#endif
