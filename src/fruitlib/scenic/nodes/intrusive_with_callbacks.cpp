#include "intrusive_with_callbacks.hpp"

fruitcut::fruitlib::scenic::nodes::intrusive_with_callbacks::intrusive_with_callbacks(
	update_callback const &_update,
	render_callback const &_render)
:
	update_(
		_update),
	render_(
		_render)
{
}

fruitcut::fruitlib::scenic::nodes::intrusive_with_callbacks::~intrusive_with_callbacks()
{
}

void
fruitcut::fruitlib::scenic::nodes::intrusive_with_callbacks::update()
{
	if (update_)
		update_();
}

void
fruitcut::fruitlib::scenic::nodes::intrusive_with_callbacks::render()
{
	if (render_)
		render_();
}
