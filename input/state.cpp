#include "state.hpp"
#include "state_manager.hpp"

fruitcut::input::state::state(
	state_manager &_manager)
:
	manager_(
		_manager)
{
	manager_.add(
		*this);
}

fruitcut::input::state_connection const
fruitcut::input::state::create_connection(
	optional_key_callback const &a,
	optional_mouse_axis_callback const &b,
	optional_mouse_button_callback const &c)
{
	return 
		std::make_shared<state_connection_impl>(
			*this,
			a,
			b,
			c);
}


fruitcut::input::state::~state()
{
	manager_.remove(
		*this);
}

void
fruitcut::input::state::key_callback_internal(
	sge::input::keyboard::key_event const &e)
{
	key_signal_(e);
}

void
fruitcut::input::state::mouse_axis_callback_internal(
	sge::input::mouse::axis_event const &e)
{
	mouse_axis_signal_(e);
}

void
fruitcut::input::state::mouse_button_callback_internal(
	sge::input::mouse::button_event const &e)
{
	mouse_button_signal_(e);
}
