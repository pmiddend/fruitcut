#include "machine.hpp"
#include "states/intro.hpp"
#include <fcppt/scoped_state_machine.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/io/cerr.hpp>
#include <iostream>

int main(
	int argc,
	char *argv[])
try
{
	fruitcut::app::machine m(
		argc,
		argv);

	fcppt::scoped_state_machine<fruitcut::app::machine> sm(
		m);

	m.run();
}
catch (fcppt::exception const &e)
{
	fcppt::io::cerr 
		<< FCPPT_TEXT("fcppt::exception: ") 
		<< e.string() 
		<< FCPPT_TEXT("\n");
	return EXIT_FAILURE;
}
catch (std::exception const &e)
{
	std::cerr 
		<< "std::exception: " 
		<< e.what() 
		<< "\n";
}
