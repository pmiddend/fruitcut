#include "light_source_from_json.hpp"
#include "../fruitlib/json/find_and_convert_member.hpp"

fruitcut::app::directional_light_source const
fruitcut::app::light_source_from_json(
	sge::parse::json::object const &o)
{
	app::directional_light_source result;
	result.position(
		fruitlib::json::find_and_convert_member<sge::renderer::vector3>(
			o,
			fruitlib::json::path(
				FCPPT_TEXT("position"))));
	result.transformation(
		fruitlib::json::find_and_convert_member<sge::renderer::matrix4>(
			o,
			fruitlib::json::path(
				FCPPT_TEXT("transformation"))));
	return result;
}
