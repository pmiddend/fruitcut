#include <fruitapp/load_user_config.hpp>
#include <fruitapp/name.hpp>
#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <sge/config/config_path.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_string_exn.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <fcppt/filesystem/path.hpp>


sge::parse::json::object const
fruitapp::load_user_config(
	sge::charconv::system &charconv_system)
{
	fcppt::filesystem::path const final_name =
		sge::config::config_path(
			fruitapp::name())/
		FCPPT_TEXT("config.json");

	return
		fcppt::filesystem::exists(
			final_name)
		?
			sge::parse::json::parse_string_exn(
				fruitlib::utf8_file_to_fcppt_string(
					charconv_system,
					final_name))
		:
			sge::parse::json::object();
}
