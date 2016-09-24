#include <fruitlib/signal_stack_printer/object.hpp>
#include <fcppt/config/compiler.hpp>
#include <fcppt/config/platform.hpp>
#include <fcppt/make_unique_ptr.hpp>

// TODO: Add own check for this
#if defined(FCPPT_CONFIG_POSIX_PLATFORM) && defined(FCPPT_CONFIG_GCC_COMPILER)
#include <fcppt/backtrace/print_current_stack_frame.hpp>
#include <fcppt/config/external_begin.hpp>
#include <signal.h>
#include <cstdlib>
#include <ostream>
#include <iostream>
#include <fcppt/config/external_end.hpp>

namespace fruitlib
{
namespace signal_stack_printer
{
class object_impl
{
FCPPT_NONCOPYABLE(
	object_impl);
public:
	explicit
	object_impl()
	:
		signal_type_(
			SIGSEGV),
		sigaction_(),
		old_sigaction_()
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
		sigaction_.sa_flags = SA_SIGINFO;

		sigaction(
			signal_type_,
			&sigaction_,
			// Old action pointer
			&old_sigaction_);
	}

	~object_impl()
	{
		sigaction(
			signal_type_,
			&old_sigaction_,
			// Old action pointer
			nullptr);
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
		std::exit(
			EXIT_FAILURE);
	}
};
}
}
#else
namespace fruitlib
{
namespace signal_stack_printer
{
class object_impl
{
};
}
}
#endif

fruitlib::signal_stack_printer::object::object()
:
	impl_(
		fcppt::make_unique_ptr<fruitlib::signal_stack_printer::object_impl>())
{
}

fruitlib::signal_stack_printer::object::~object()
{
}
