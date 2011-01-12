#include "state_manager.hpp"
#include "state.hpp"
#include <sge/systems/instance.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/mouse/collector.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/input/keyboard/key.hpp>
#include <fcppt/assert.hpp>
#include <boost/bind.hpp>
#include <iostream>

fruitcut::input::state_manager::state_manager(
	sge::systems::instance const &systems)
:
	keyboard_(
		*systems.keyboard_collector()),
	c1(
		systems.keyboard_collector()->key_callback(
			boost::bind(
				&state_manager::key_callback_internal,
				this,
				_1))),
	c2(
		systems.mouse_collector()->axis_callback(
			boost::bind(
				&state_manager::mouse_axis_callback_internal,
				this,
				_1))),
	c3(
		systems.mouse_collector()->button_callback(
			boost::bind(
				&state_manager::mouse_button_callback_internal,
				this,
				_1))),
	current_state_(
		0)
{
}

void
fruitcut::input::state_manager::current_state(
	state &_current_state)
{
	current_state_ = &_current_state;
}

fruitcut::input::state const *
fruitcut::input::state_manager::current_state() const
{
	return current_state_;
}

fruitcut::input::state_manager::~state_manager()
{
}

void
fruitcut::input::state_manager::add(
	state const &)
{
}

void
fruitcut::input::state_manager::remove(
	state const &s)
{
	if (&s == current_state_)
		current_state_ = 0;
}

void
fruitcut::input::state_manager::key_callback_internal(
	sge::input::keyboard::key_event const &c)
{
	if (current_state_)
	{
		std::cout << "There is a current state\n";
		current_state_->key_callback_internal(
			c);
	}
}


void
fruitcut::input::state_manager::mouse_axis_callback_internal(
	sge::input::mouse::axis_event const &e)
{
	if (current_state_)
		current_state_->mouse_axis_callback_internal( 
			e);
}

void
fruitcut::input::state_manager::mouse_button_callback_internal(
	sge::input::mouse::button_event const &b)
{
	if (current_state_)
		current_state_->mouse_button_callback_internal( 
			b);
}


