#include "from_json.hpp"
#include <fruitlib/json/find_and_convert_member.hpp>

fruitapp::fruit::material::object const
fruitapp::fruit::material::from_json(
	sge::parse::json::object const &o)
{
	return
		material::object(
			material::diffuse_color(
				fruitlib::json::find_and_convert_member<material::diffuse_color::value_type>(
					o,
					fruitlib::json::path(
						FCPPT_TEXT("diffuse-color")))),
			material::diffuse_coefficient(
				fruitlib::json::find_and_convert_member<material::diffuse_coefficient::value_type>(
					o,
					fruitlib::json::path(
						FCPPT_TEXT("diffuse-coefficient")))),
			material::specular_color(
				fruitlib::json::find_and_convert_member<material::diffuse_color::value_type>(
					o,
					fruitlib::json::path(
						FCPPT_TEXT("specular-color")))),
			material::specular_coefficient(
				fruitlib::json::find_and_convert_member<material::specular_coefficient::value_type>(
					o,
					fruitlib::json::path(
						FCPPT_TEXT("specular-coefficient")))),
			material::specular_shininess(
				fruitlib::json::find_and_convert_member<material::specular_shininess::value_type>(
					o,
					fruitlib::json::path(
						FCPPT_TEXT("specular-shininess")))));
}
