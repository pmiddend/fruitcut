#include "state.hpp"
#include "state_manager.hpp"
#include <sge/input/keyboard/mod_state.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <iostream>

fruitcut::fruitlib::input::state::state(
	state_manager &_manager)
:
	manager_(
		_manager)
{
	manager_.add(
		*this);
}

fcppt::signal::auto_connection
fruitcut::fruitlib::input::state::button_callback(
	sge::input::mouse::button_callback const &e)
{
	return mouse_button_signal_.connect(e);
}

fcppt::signal::auto_connection
fruitcut::fruitlib::input::state::axis_callback(
	sge::input::mouse::axis_callback const &e)
{
	return mouse_axis_signal_.connect(e);
}

fcppt::signal::auto_connection
fruitcut::fruitlib::input::state::key_callback(
	sge::input::keyboard::key_callback const &e)
{
	return key_signal_.connect(e);
}

fcppt::signal::auto_connection
fruitcut::fruitlib::input::state::key_repeat_callback(
	sge::input::keyboard::key_repeat_callback const &e)
{
	return key_repeat_signal_.connect(e);
}

fcppt::signal::auto_connection
fruitcut::fruitlib::input::state::char_callback(
	sge::input::keyboard::char_callback const &e)
{
	return char_signal_.connect(e);
}

sge::input::keyboard::mod_state const
fruitcut::fruitlib::input::state::mod_state() const
{
	return manager_.keyboard_.mod_state();
}

fruitcut::fruitlib::input::state::~state()
{
	manager_.remove(
		*this);
}

void
fruitcut::fruitlib::input::state::key_callback_internal(
	sge::input::keyboard::key_event const &e)
{
	key_signal_(e);
}

void
fruitcut::fruitlib::input::state::key_repeat_callback_internal(
	sge::input::keyboard::key_repeat_event const &e)
{
	key_repeat_signal_(e);
}

void
fruitcut::fruitlib::input::state::char_callback_internal(
	sge::input::keyboard::char_event const &e)
{
	char_signal_(e);
}

void
fruitcut::fruitlib::input::state::mouse_axis_callback_internal(
	sge::input::mouse::axis_event const &e)
{
	mouse_axis_signal_(e);
}

void
fruitcut::fruitlib::input::state::mouse_button_callback_internal(
	sge::input::mouse::button_event const &e)
{
	mouse_button_signal_(e);
}
