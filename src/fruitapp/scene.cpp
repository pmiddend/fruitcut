#include <fruitapp/scene.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <fruitlib/scenic/events/render.hpp>


fruitapp::scene::scene(
	fruitlib::scenic::optional_parent const &_parent)
:
	node_base(
		_parent),
	active_(
		true)
{
}

void
fruitapp::scene::active(
	bool const _active)
{
	active_ = _active;
}

bool
fruitapp::scene::active() const
{
	return active_;
}

void
fruitapp::scene::react(
	fruitlib::scenic::events::update const &e)
{
	if(active_)
		node_base::forward_to_children(
			e);
}

void
fruitapp::scene::render_children(
	sge::renderer::context::core &_context)
{
	if(active_)
	{
		fruitlib::scenic::events::render event(
			dynamic_cast<sge::renderer::context::ffp &>(
				_context));

		node_base::forward_to_children(
			event);
	}
}

fruitapp::scene::~scene()
{
}
