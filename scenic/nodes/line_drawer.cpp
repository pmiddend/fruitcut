#include "line_drawer.hpp"
#include <sge/line_drawer/object.hpp>
#include <sge/line_drawer/render_to_screen.hpp>

fruitcut::scenic::nodes::line_drawer::line_drawer(
	sge::line_drawer::object &_object,
	sge::renderer::device_ptr const _renderer)
:
	object_(
		_object),
	renderer_(
		_renderer)
{
}

fruitcut::scenic::nodes::line_drawer::~line_drawer()
{
}

void
fruitcut::scenic::nodes::line_drawer::render()
{
	if(renderer_)
		sge::line_drawer::render_to_screen(
			renderer_,
			object_);
	else
		object_.render();
}

void
fruitcut::scenic::nodes::line_drawer::update()
{
}
