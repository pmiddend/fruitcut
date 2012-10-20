#include <fruitapp/load_user_config.hpp>
#include <fruitapp/name.hpp>
#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <sge/config/config_path.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_string_exn.hpp>
#include <fcppt/text.hpp>


sge::parse::json::object const
fruitapp::load_user_config(
	sge::charconv::system &charconv_system)
{
	fcppt::optional<fcppt::string> const optional_json_string =
		fruitlib::utf8_file_to_fcppt_string(
			charconv_system,
			sge::config::config_path(
				fruitapp::name()) /
			FCPPT_TEXT("config.json"));

	return
		optional_json_string
		?
			sge::parse::json::parse_string_exn(
				*optional_json_string).object()
		:
			sge::parse::json::object();
}
