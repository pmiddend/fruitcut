#include "load_user_config.hpp"
#include "name.hpp"
#include <sge/config/config_path.hpp>
#include <sge/parse/json/json.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/exists.hpp>

sge::parse::json::object const
fruitcut::app::load_user_config()
{
	fcppt::filesystem::path const final_name = 
		sge::config::config_path(
			app::name())/
		FCPPT_TEXT("config.json");

	return 
		fcppt::filesystem::exists(
			final_name)
		?
			sge::parse::json::parse_file_exn(
				final_name)
		:
			sge::parse::json::object();
}
