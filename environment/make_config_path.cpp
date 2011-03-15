#include "make_config_path.hpp"
#include "optional_string.hpp"
#include "getenv.hpp"
#include <fcppt/text.hpp>
#include <fcppt/config.hpp>

fcppt::filesystem::path const
fruitcut::environment::make_config_path(
	fcppt::string const &app_name,
	fcppt::string const &desired_filename)
{
#if defined(FCPPT_WINDOWS_PLATFORM)
	return
		fcppt::filesystem::path(
			config::getenv_exn(
				FCPPT_TEXT("APPDIR"))) 
			/ _appname;
#elif defined(FCPPT_POSIX_PLATFORM)
	optional_string const xdg_home = 
		getenv(
			FCPPT_TEXT("XDG_CONFIG_HOME"));

	if(xdg_home && !(xdg_home->empty()))
	{
		return 
			fcppt::filesystem::path(*xdg_home)
				/ app_name
				/ desired_filename;
	}

	return 
		fcppt::filesystem::path(*getenv(FCPPT_TEXT("HOME"))) 
			/ FCPPT_TEXT(".config") 
			/ app_name 
			/ desired_filename;
#else
#error "don't know how to find a config path"
#endif
}
