#ifndef FRUITCUT_INPUT_STATE_HPP_INCLUDED
#define FRUITCUT_INPUT_STATE_HPP_INCLUDED

#include "state_manager_fwd.hpp"
#include "state_connection.hpp"
#include "optional_key_callback.hpp"
#include "optional_mouse_axis_callback.hpp"
#include "optional_mouse_button_callback.hpp"
#include <sge/input/keyboard/key_callback.hpp>
#include <sge/input/keyboard/key_function.hpp>
#include <sge/input/mouse/axis_callback.hpp>
#include <sge/input/mouse/button_callback.hpp>
#include <sge/input/mouse/axis_function.hpp>
#include <sge/input/mouse/button_function.hpp>
#include <fcppt/signal/object.hpp>

namespace fruitcut
{
namespace input
{
class state
{
public:
	state(state const &) = delete;
	state &operator=(state const &) = delete;

	explicit
	state(
		state_manager &);

	state_connection const
	create_connection(
		optional_key_callback const &,
		optional_mouse_axis_callback const &,
		optional_mouse_button_callback const &);

	~state();
private:
	friend class state_manager;
	friend class state_connection_impl;

	typedef 
	fcppt::signal::object<sge::input::keyboard::key_function> 
	key_signal;

	typedef 
	fcppt::signal::object<sge::input::mouse::axis_function>
	mouse_axis_signal;

	typedef 
	fcppt::signal::object<sge::input::mouse::button_function>
	mouse_button_signal;

	key_signal key_signal_;
	mouse_axis_signal mouse_axis_signal_;
	mouse_button_signal mouse_button_signal_;
	
	state_manager &manager_;

	// Called by state_manager
	void
	key_callback_internal(
		sge::input::keyboard::key_event const &);

	void
	mouse_axis_callback_internal(
		sge::input::mouse::axis_event const &);

	void
	mouse_button_callback_internal(
		sge::input::mouse::button_event const &);
};
}
}

#endif
