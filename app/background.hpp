#ifndef FRUITCUT_APP_BACKGROUND_HPP_INCLUDED
#define FRUITCUT_APP_BACKGROUND_HPP_INCLUDED

#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>

namespace fruitcut
{
namespace app
{
// The background is separate because
// a) it is immortal
// b) it has repetition
// c) it has additional logic (splatting!)
class background
{
public:
	explicit
	background(
		sge::renderer::device_ptr,
		sge::image2d::multi_loader &,
		sge::parse::json::object const &);

	void
	render();

	void
	viewport_changed();

	~background();
private:
	sge::renderer::device_ptr renderer_;
	sge::renderer::texture::planar_ptr texture_;
	sge::renderer::vertex_buffer_ptr vb_;
};
}
}

#endif