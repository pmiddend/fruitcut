#include <fruitapp/load_user_config.hpp>
#include <fruitapp/name.hpp>
#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <fruitlib/json/parse_string_exn.hpp>
#include <sge/config/config_path.hpp>
#include <sge/parse/json/json.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/exists.hpp>

sge::parse::json::object const
fruitapp::load_user_config()
{
	fcppt::filesystem::path const final_name = 
		sge::config::config_path(
			fruitapp::name())/
		FCPPT_TEXT("config.json");

	return 
		fcppt::filesystem::exists(
			final_name)
		?
			fruitlib::json::parse_string_exn(
				fruitlib::utf8_file_to_fcppt_string(
					final_name))
		:
			sge::parse::json::object();
}
