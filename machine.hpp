#ifndef FRUITCUT_MACHINE_HPP_INCLUDED
#define FRUITCUT_MACHINE_HPP_INCLUDED

#include "states/running_fwd.hpp"
#include "input/state.hpp"
#include "input/state_manager.hpp"
#include "graphics/camera/object.hpp"
#include <sge/systems/instance.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state_machine.hpp>

namespace fruitcut
{
class machine
:
	public 
		boost::statechart::state_machine
		<
			machine,
			states::running
		>
{
public:
	explicit
	machine(
		int argc,
		char *argv[]);

	bool
	dead() const;

	void
	run_once();

	sge::parse::json::object const &
	config_file() const;

	sge::systems::instance const &
	systems() const;

	graphics::camera::object const &
	camera() const;

	// We need the non-const version to call update() in the child
	// state's tick function
	graphics::camera::object &
	camera();
private:
	sge::parse::json::object const config_file_;
	sge::systems::instance const systems_;
	input::state_manager input_manager_;
	input::state running_input_state_;
	bool dead_;
	fcppt::signal::scoped_connection escape_connection_;
	graphics::camera::object camera_;
	sge::time::timer frame_timer_;

	void
	render();
};
}

#endif
