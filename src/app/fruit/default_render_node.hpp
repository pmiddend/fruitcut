#ifndef FRUITCUT_APP_FRUIT_DEFAULT_RENDER_NODE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_DEFAULT_RENDER_NODE_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "../directional_light_source_fwd.hpp"
#include "../../fruitlib/scenic/nodes/intrusive.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vertex_declaration_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class default_render_node
:
	public fruitlib::scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	default_render_node);
public:
	explicit
	default_render_node(
		sge::renderer::device &,
		sge::renderer::vertex_declaration &,
		fruit::manager const &,
		sge::camera::object &,
		app::directional_light_source const &,
		sge::renderer::scalar);

	~default_render_node();
private:
	sge::renderer::device &renderer_;
	fruit::manager const &manager_;
	sge::camera::object const &camera_;
	sge::shader::object shader_;

	void
	render();

	void
	update();
};
}
}
}

#endif
