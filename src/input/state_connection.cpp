#include "state_connection.hpp"
#include "state.hpp"
#include <sge/systems/instance.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/mouse/collector.hpp>
#include <fcppt/signal/auto_connection.hpp>

fruitcut::input::state_connection_impl::state_connection_impl(
	state &s,
	optional_key_callback const &a,
	optional_mouse_axis_callback const &b,
	optional_mouse_button_callback const &c)
:
	c1(
		a
		?
			s.key_signal_.connect(
				*a)
		:
			fcppt::signal::auto_connection()),
	c2(
		b
		?
			s.mouse_axis_signal_.connect(
				*b)
		:
			fcppt::signal::auto_connection()),
	c3(
		c
		?
			s.mouse_button_signal_.connect(
				*c)
		:
			fcppt::signal::auto_connection())
{
}
