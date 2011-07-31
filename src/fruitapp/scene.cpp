#include "scene.hpp"
#include "../fruitlib/scenic/events/render.hpp"
#include "../fruitlib/scenic/events/update.hpp"

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
fruitapp::scene::render_children()
{
	if(active_)
		node_base::forward_to_children(
			fruitlib::scenic::events::render());
}
