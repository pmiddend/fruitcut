#ifndef FRUITCUT_INPUT_STATE_HPP_INCLUDED
#define FRUITCUT_INPUT_STATE_HPP_INCLUDED

#include "state_manager_fwd.hpp"
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_callback.hpp>
#include <sge/input/keyboard/key_function.hpp>
#include <sge/input/keyboard/key_repeat_callback.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <sge/input/keyboard/key_repeat_event_fwd.hpp>
#include <sge/input/keyboard/key_repeat_function.hpp>
#include <sge/input/keyboard/key_fwd.hpp>
#include <sge/input/keyboard/mod_state.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/input/mouse/axis_callback.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <sge/input/mouse/button_callback.hpp>
#include <sge/input/mouse/axis_function.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
#include <sge/input/mouse/button_function.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace input
{
class state
:
	public sge::input::mouse::device,
	public sge::input::keyboard::device
{
FCPPT_NONCOPYABLE(state)
public:
	explicit
	state(
		state_manager &);

	fcppt::signal::auto_connection
	button_callback(
		sge::input::mouse::button_callback const &);

	fcppt::signal::auto_connection
	axis_callback(
		sge::input::mouse::axis_callback const &);

	fcppt::signal::auto_connection
	key_callback(
		sge::input::keyboard::key_callback const &);

	// NOT IMPLEMENTED YET!
	fcppt::signal::auto_connection
	key_repeat_callback(
		sge::input::keyboard::key_repeat_callback const &);

	// NOT IMPLEMENTED YET!
	sge::input::keyboard::mod_state const
	mod_state() const;

	~state();
private:
	friend class state_manager;

	typedef 
	fcppt::signal::object<sge::input::keyboard::key_function> 
	key_signal;

	typedef 
	fcppt::signal::object<sge::input::keyboard::key_repeat_function> 
	key_repeat_signal;

	typedef 
	fcppt::signal::object<sge::input::mouse::axis_function>
	mouse_axis_signal;

	typedef 
	fcppt::signal::object<sge::input::mouse::button_function>
	mouse_button_signal;

	key_signal key_signal_;
	key_repeat_signal key_repeat_signal_;
	mouse_axis_signal mouse_axis_signal_;
	mouse_button_signal mouse_button_signal_;
	state_manager &manager_;

	// Called by state_manager
	void
	key_callback_internal(
		sge::input::keyboard::key_event const &);

	void
	key_repeat_callback_internal(
		sge::input::keyboard::key_repeat_event const &);

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
