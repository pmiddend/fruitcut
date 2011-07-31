#include "machine.hpp"
#include "states/loading.hpp"
#include <fcppt/scoped_state_machine.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/config.hpp>
#include <fcppt/platform.hpp>
#include <iostream>

#include <boost/filesystem/operations.hpp>

#if defined(FCPPT_HAVE_BACKTRACE) && defined(FCPPT_POSIX_PLATFORM)
#define FRUITCUT_HAVE_SIGNAL_STACK_PRINTER
#include <fcppt/noncopyable.hpp>
#include <fcppt/backtrace/print_current_stack_frame.hpp>
#include <signal.h>
#include <cstdlib>

namespace
{
class signal_stack_printer
{
FCPPT_NONCOPYABLE(
	signal_stack_printer);
public:
	explicit
	signal_stack_printer(
		int const _signal_type)
	:
		signal_type_(
			_signal_type)
	{
		sigaction_.sa_sigaction = 
			&callback;
		// sa_mask specifies which signals should be blocked during the
		// handler execution
		sigemptyset(
			&sigaction_.sa_mask);
		// SA_SIGINFO: Ignore sa_handler (which lets you set "ignore" and
		// "use default handler") and use sa_sigaction as callback.
		// SA_RESTART: I don't get what it does
		sigaction_.sa_flags = SA_RESTART | SA_SIGINFO;

		sigaction(
			signal_type_,
			&sigaction_,
			// Old action pointer
			&old_sigaction_);
	}

	~signal_stack_printer()
	{
		sigaction(
			signal_type_,
			&old_sigaction_,
			// Old action pointer
			0);
	}
private:
	int const signal_type_;
	struct sigaction sigaction_;
	struct sigaction old_sigaction_;

	static void 
	callback(
		int /*signal*/,
		siginfo_t */*info*/,
		void *)
	{
		std::cerr << "Got a SIGSEGV, printing backtrace:\n";
		fcppt::backtrace::print_current_stack_frame();
		std::exit(-1);
	}
};
}
#endif

int main(
	int argc,
	char *argv[])
try
{
	boost::filesystem3::directory_iterator it,it2(it);
#if defined(FRUITCUT_HAVE_SIGNAL_STACK_PRINTER)
	signal_stack_printer stack_printer(
		SIGSEGV);
#endif

	fruitapp::machine m(
		argc,
		argv);

	fcppt::scoped_state_machine<fruitapp::machine> sm(
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
