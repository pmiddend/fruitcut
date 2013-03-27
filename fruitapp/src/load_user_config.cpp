#include <fruitapp/load_user_config.hpp>
#include <fruitapp/name.hpp>
#include <sge/charconv/utf8_file_to_fcppt_string.hpp>
#include <sge/config/app_name.hpp>
#include <sge/config/config_path.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_string_exn.hpp>
#include <fcppt/optional_string.hpp>
#include <fcppt/text.hpp>


sge::parse::json::object const
fruitapp::load_user_config()
{
	fcppt::optional_string const optional_json_string =
		sge::charconv::utf8_file_to_fcppt_string(
			sge::config::config_path(
				sge::config::app_name(
					fruitapp::name())) /
			FCPPT_TEXT("config.json"));

	return
		optional_json_string
		?
			sge::parse::json::parse_string_exn(
				*optional_json_string).object()
		:
			sge::parse::json::object();
}
