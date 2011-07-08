#ifndef FRUITCUT_APP_BACKGROUND_HPP_INCLUDED
#define FRUITCUT_APP_BACKGROUND_HPP_INCLUDED

#include "../fruitlib/scenic/nodes/intrusive.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/signal/scoped_connection.hpp>

namespace fruitcut
{
namespace app
{
class background
:
	public fruitlib::scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	background);
public:
	explicit
	background(
		sge::renderer::device &,
		sge::viewport::manager &,
		sge::image2d::multi_loader &,
		sge::renderer::texture::planar_ptr,
		sge::renderer::matrix4 const &,
		sge::parse::json::object const &,
		sge::camera::object const &);

	void
	viewport_changed();

	~background();
private:
	sge::renderer::device &renderer_;
	sge::camera::object const &camera_;
	sge::renderer::texture::planar_ptr texture_;
	sge::renderer::vertex_declaration_ptr vertex_declaration_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::shader::object shader_;
	sge::renderer::scalar const reps_;
	fcppt::signal::scoped_connection viewport_changed_connection_;

	void
	render();

	void
	update();
};
}
}

#endif
