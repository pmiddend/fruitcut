#ifndef FRUITCUT_APP_BACKGROUND_HPP_INCLUDED
#define FRUITCUT_APP_BACKGROUND_HPP_INCLUDED

#include "../scenic/nodes/intrusive.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/vertex_declaration_ptr.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
// The background is separate because
// a) it is immortal
// b) it has repetition
// c) it has additional logic (splatting!)
class background
:
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	background);
public:
	explicit
	background(
		sge::renderer::device &,
		sge::image2d::multi_loader &,
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

	void
	render();

	void
	update();
};
}
}

#endif
