#ifndef FRUITAPP_BACKGROUND_BASE_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_BASE_HPP_INCLUDED

#include <fruitapp/background/repetitions.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fruitlib/perspective_projection_information.hpp>
#include <fruitlib/texture_manager_fwd.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/renderer/context/core_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/renderer/state/core/depth_stencil/object_unique_ptr.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <sge/renderer/texture/planar_unique_ptr.hpp>
#include <sge/renderer/vertex/buffer_unique_ptr.hpp>
#include <sge/renderer/vertex/declaration_unique_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>


namespace fruitapp
{
namespace background
{
class base
{
FCPPT_NONCOPYABLE(
	base);
public:
	virtual void
	render(
		sge::renderer::context::core &) = 0;

	virtual ~base() = 0;
protected:
	sge::renderer::target::base const &target_;
	sge::renderer::vertex::declaration_unique_ptr const vertex_declaration_;
	sge::renderer::vertex::buffer_unique_ptr const vb_;
	sge::renderer::texture::planar_unique_ptr const texture_;
	fcppt::signal::scoped_connection const projection_change_connection_;
	sge::renderer::state::core::depth_stencil::object_unique_ptr const depth_stencil_state_;

	base(
		sge::renderer::device::core &,
		fruitlib::texture_manager &,
		sge::camera::base const &,
		fruitapp::projection_manager::object &,
		fruitapp::background::repetitions const &);

	void
	do_render(
		sge::renderer::context::core &);

	sge::renderer::texture::planar &
	texture();
private:
	void
	projection_change(
		sge::camera::base const &,
		fruitapp::background::repetitions const &,
		fruitlib::perspective_projection_information const &);
};
}
}

#endif
