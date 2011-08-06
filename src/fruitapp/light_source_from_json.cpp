#include <fruitapp/light_source_from_json.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

fruitapp::directional_light_source const
fruitapp::light_source_from_json(
	sge::parse::json::object const &o)
{
	fruitapp::directional_light_source result;
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
