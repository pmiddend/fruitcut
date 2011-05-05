#include "make_cache_path.hpp"
#include "optional_string.hpp"
#include "getenv.hpp"
#include <fcppt/text.hpp>
#include <fcppt/platform.hpp>

fcppt::filesystem::path const
fruitcut::environment::make_cache_path(
	fcppt::string const &app_name,
	fcppt::string const &desired_filename)
{
#if defined(FCPPT_WINDOWS_PLATFORM)
	return
		fcppt::filesystem::path(
			*getenv(
				FCPPT_TEXT("APPDIR"))) 
			/ app_name);
#elif defined(FCPPT_POSIX_PLATFORM)
	optional_string const xdg_home = 
		getenv(
			FCPPT_TEXT("XDG_CACHE_HOME"));

	if(xdg_home && !(xdg_home->empty()))
	{
		return 
			fcppt::filesystem::path(*xdg_home)
				/ app_name
				/ desired_filename;
	}

	return 
		fcppt::filesystem::path(*getenv(FCPPT_TEXT("HOME"))) 
			/ FCPPT_TEXT(".cache") 
			/ app_name 
			/ desired_filename;
#else
#error "don't know how to find a config path"
#endif
}
