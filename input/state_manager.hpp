#ifndef FRUITCUT_INPUT_STATE_MANAGER_HPP_INCLUDED
#define FRUITCUT_INPUT_STATE_MANAGER_HPP_INCLUDED

#include "state_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/input/keyboard/key_function.hpp>
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/input/mouse/axis_function.hpp>
#include <sge/input/mouse/button_function.hpp>
#include <sge/input/keyboard/key_callback.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <sge/input/mouse/axis_callback.hpp>
#include <sge/input/mouse/button_callback.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/signal/object.hpp>

namespace fruitcut
{
namespace input
{
// As the name suggests, the state manager handles "input
// states". Every application is composed of at least one state. A
// game, for example, might be composed of the state "menu", the state
// "ingame" and the state "console open". In each state, different
// objects would like to receive input events. The state manager,
// along with the state and state_connection class, provides a
// facility to express just that. 
// 
// In the above example, you would create three state objects. You
// would connect the console object to the console state, the menu
// object to the menu state and the ingame object(s) (or something) to
// the ingame state.
class state_manager
{
public:
	// Initialized current_state to 0. It stays that way until someone
	// calls current_state
	explicit 
	state_manager(
		sge::systems::instance const &);

	void
	current_state(
		state &);

	// Can, of course, be 0
	state const *
	current_state() const;

	// Can, of course, be 0
	state *
	current_state();

	~state_manager();
private:
	friend class state;

	sge::input::keyboard::device &keyboard_;
	fcppt::signal::scoped_connection c1,c2,c3;

	// We don't store all the states, but we keep track of destruction,
	// so _this_ variable doesn't get invalid
	state *current_state_;

	// Called internally by state
	void
	add(
		state const &);

	// Called internally by state
	void
	remove(
		state const &);

	// Convenience, do not call in state!
	void
	add(
		state const *);

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
