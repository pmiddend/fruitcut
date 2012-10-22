#include <fruitapp/machine.hpp>
#include <fruitapp/main.hpp>
#include <fruitapp/states/loading.hpp>
#include <fruitlib/signal_stack_printer/object.hpp>
#include <fruitlib/message_box.hpp>
#include <awl/main/exit_code.hpp>
#include <awl/main/exit_failure.hpp>
#include <awl/main/function_context.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/scoped_state_machine.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


awl::main::exit_code const
fruitapp::main(
	awl::main::function_context const &_main_function_context)
try
{
	fruitlib::signal_stack_printer::object stack_printer;

	fruitapp::machine machine(
		_main_function_context.argc(),
		_main_function_context.argv());

	fcppt::scoped_state_machine<fruitapp::machine> scoped_state_machine(
		machine);

	return
		machine.run();
}
catch (fcppt::exception const &e)
{
	fruitlib::message_box(
		FCPPT_TEXT("fcppt::exception: ")+
		e.string());
	return awl::main::exit_failure();
}
catch (std::exception const &e)
{
	fruitlib::message_box(
		"std::exception: "+
		fcppt::from_std_string(
			e.what()));
	return awl::main::exit_failure();
}
