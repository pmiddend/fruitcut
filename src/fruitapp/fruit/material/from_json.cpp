#include <fruitapp/fruit/material/from_json.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>


fruitapp::fruit::material::object const
fruitapp::fruit::material::from_json(
	sge::parse::json::object const &o)
{
	return
		material::object(
			material::diffuse_color(
				sge::parse::json::find_and_convert_member<material::diffuse_color::value_type>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("diffuse-color")))),
			material::diffuse_coefficient(
				sge::parse::json::find_and_convert_member<material::diffuse_coefficient::value_type>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("diffuse-coefficient")))),
			material::specular_color(
				sge::parse::json::find_and_convert_member<material::diffuse_color::value_type>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("specular-color")))),
			material::specular_coefficient(
				sge::parse::json::find_and_convert_member<material::specular_coefficient::value_type>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("specular-coefficient")))),
			material::specular_shininess(
				sge::parse::json::find_and_convert_member<material::specular_shininess::value_type>(
					o,
					sge::parse::json::path(
						FCPPT_TEXT("specular-shininess")))));
}
