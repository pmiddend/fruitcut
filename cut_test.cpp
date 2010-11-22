#include "input/state_manager.hpp"
#include "input/state.hpp"
#include "input/optional_key_callback.hpp"
#include "input/optional_mouse_axis_callback.hpp"
#include "input/optional_mouse_button_callback.hpp"
#include "json/config_wrapper.hpp"
#include "json/find_member.hpp"
#include "machine.hpp"
// Has to be here, first state
#include "states/freelook.hpp"
#include <sge/log/global.hpp>
#include <sge/exception.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/scoped_state_machine.hpp>
#include <exception>
#include <iostream>
#include <ostream>

int main(int argc,char *argv[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	fruitcut::machine m(
		argc,
		argv);

	fcppt::scoped_state_machine<fruitcut::machine> scoped_machine(
		m);

	while (!m.dead())
		m.run_once();
}
catch(sge::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch (fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	std::cerr << e.what() << '\n';
	return EXIT_FAILURE;
}
