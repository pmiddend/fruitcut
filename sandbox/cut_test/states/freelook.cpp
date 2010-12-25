#include "freelook.hpp"
#include "cut.hpp"
#include "../input_states.hpp"
#include <fcppt/io/cout.hpp>

fruitcut::sandbox::cut_test::states::freelook::freelook(
	my_context const ctx)
:
	my_base(
		ctx)
{
	context<ingame>().current_input_state(
		input_states::freelook);
}

boost::statechart::result
fruitcut::sandbox::cut_test::states::freelook::react(
	events::tick const &t)
{
	context<ingame>().camera().update(
		t.time_delta());
	return discard_event();
}

boost::statechart::result
fruitcut::sandbox::cut_test::states::freelook::react(
	events::toggle_mode const &)
{
	return transit<cut>();
}
