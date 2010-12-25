#ifndef FRUITCUT_SANDBOX_CUT_TEST_MACHINE_HPP_INCLUDED
#define FRUITCUT_SANDBOX_CUT_TEST_MACHINE_HPP_INCLUDED

#include "states/ingame_fwd.hpp"
#include "../../input/state.hpp"
#include "../../input/state_manager.hpp"
#include <sge/systems/instance.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/time/timer.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state_machine.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace cut_test
{
class machine
:
	public 
		boost::statechart::state_machine
		<
			machine,
			states::ingame
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
private:
	sge::parse::json::object const config_file_;
	sge::systems::instance const systems_;
	bool dead_;
	fcppt::signal::scoped_connection escape_connection_;
	sge::time::timer frame_timer_;

	void
	render();
};
}
}
}

#endif
