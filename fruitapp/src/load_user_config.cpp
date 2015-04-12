#include <fruitapp/load_user_config.hpp>
#include <fruitapp/name.hpp>
#include <sge/charconv/utf8_file_to_fcppt_string.hpp>
#include <sge/config/app_name.hpp>
#include <sge/config/config_path.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_string_exn.hpp>
#include <fcppt/maybe.hpp>
#include <fcppt/optional_string.hpp>
#include <fcppt/text.hpp>


sge::parse::json::object
fruitapp::load_user_config()
{
	fcppt::optional_string const optional_json_string =
		sge::charconv::utf8_file_to_fcppt_string(
			sge::config::config_path(
				sge::config::app_name(
					fruitapp::name())) /
			FCPPT_TEXT("config.json"));

	return
		fcppt::maybe(
			optional_json_string,
			[]{
				return
					sge::parse::json::object();
			},
			[](
				fcppt::string const &_json_string
			)
			{
				return
					sge::parse::json::parse_string_exn(
						_json_string
					).object();
			}
		);
}
