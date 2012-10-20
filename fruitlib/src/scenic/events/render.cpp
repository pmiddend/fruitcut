#include <fruitlib/scenic/events/render.hpp>


fruitlib::scenic::events::render::render(
	sge::renderer::context::ffp &_context)
:
	events::base(),
	context_(
		_context)
{
}

sge::renderer::context::ffp &
fruitlib::scenic::events::render::context() const
{
	return
		context_;
}

fruitlib::scenic::events::render::~render()
{
}
