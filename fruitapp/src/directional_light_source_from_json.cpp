#include <fruitapp/directional_light_source_from_json.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/math/matrix/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>


fruitapp::directional_light_source const
fruitapp::directional_light_source_from_json(
	sge::parse::json::object const &o)
{
	fruitapp::directional_light_source result;
	result.position(
		sge::parse::json::find_and_convert_member<sge::renderer::vector3>(
			o,
			sge::parse::json::path(
				FCPPT_TEXT("position"))));
	result.transformation(
		sge::parse::json::find_and_convert_member<sge::renderer::matrix4>(
			o,
			sge::parse::json::path(
				FCPPT_TEXT("transformation"))));
	return result;
}
