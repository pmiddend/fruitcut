#include "scene.hpp"
#include "../fruitlib/scenic/events/render.hpp"
#include "../fruitlib/scenic/events/update.hpp"

fruitcut::app::scene::scene(
	fruitlib::scenic::optional_parent const &_parent)
:
	node_base(
		_parent),
	active_(
		true)
{
}

void
fruitcut::app::scene::active(
	bool const _active)
{
	active_ = _active;
}

bool
fruitcut::app::scene::active() const
{
	return active_;
}

void
fruitcut::app::scene::react(
	fruitlib::scenic::events::update const &e)
{
	if(active_)
		node_base::forward_to_children(
			e);
}

void
fruitcut::app::scene::render_children()
{
	if(active_)
		node_base::forward_to_children(
			fruitlib::scenic::events::render());
}
