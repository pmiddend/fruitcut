#include <fruitlib/message_box.hpp>
#include <fcppt/config/platform.hpp>
#include <fcppt/text.hpp>

#ifdef FCPPT_CONFIG_WINDOWS_PLATFORM
#include <fcppt/config/external_begin.hpp>
#include <windows.h>
#include <fcppt/config/external_end.hpp>
#endif

#include <fcppt/io/cerr.hpp>

void
fruitlib::message_box(
	fcppt::string const &s)
{
#ifdef FCPPT_CONFIG_WINDOWS_PLATFORM
	if(
		::MessageBox(
			NULL,
			s.c_str(),
			FCPPT_TEXT("Error"),
			MB_OK | MB_ICONERROR) == 0)
		fcppt::io::cerr() << FCPPT_TEXT("MessageBox failed!\n");
#else
	fcppt::io::cerr() << FCPPT_TEXT("Error: ") << s << FCPPT_TEXT("\n");
#endif
}
