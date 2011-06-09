#include "from_json.hpp"
#include "../../../fruitlib/json/find_member.hpp"

fruitcut::app::fruit::material::object const
fruitcut::app::fruit::material::from_json(
	sge::parse::json::object const &o)
{
	return
		material::object(
			material::diffuse_color(
				fruitlib::json::find_member<material::diffuse_color::value_type>(
					o,
					FCPPT_TEXT("diffuse-color"))),
			material::diffuse_coefficient(
				fruitlib::json::find_member<material::diffuse_coefficient::value_type>(
					o,
					FCPPT_TEXT("diffuse-coefficient"))),
			material::specular_color(
				fruitlib::json::find_member<material::diffuse_color::value_type>(
					o,
					FCPPT_TEXT("specular-color"))),
			material::specular_coefficient(
				fruitlib::json::find_member<material::specular_coefficient::value_type>(
					o,
					FCPPT_TEXT("specular-coefficient"))),
			material::specular_shininess(
				fruitlib::json::find_member<material::specular_shininess::value_type>(
					o,
					FCPPT_TEXT("specular-shininess"))));
}
