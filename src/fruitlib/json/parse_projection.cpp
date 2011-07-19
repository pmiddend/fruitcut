#include "parse_projection.hpp"
#include "find_and_convert_member.hpp"
#include <sge/camera/projection/projection.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/variant/object.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>

sge::camera::projection::object const
fruitcut::fruitlib::json::parse_projection(
	sge::parse::json::object const &o,
	fcppt::optional<sge::renderer::scalar> const &aspect)
{
	if(
		json::find_and_convert_member<fcppt::string>(
			o,
			json::path(FCPPT_TEXT("type"))) == FCPPT_TEXT("orthogonal"))
		return sge::camera::projection::orthogonal();

	return 
		sge::camera::projection::perspective(
			sge::renderer::projection::aspect(
				aspect
				?
					*aspect
				:
					json::find_and_convert_member<sge::renderer::scalar>(
						o,
						json::path(FCPPT_TEXT("aspect")))),
			sge::renderer::projection::fov(
				fcppt::math::deg_to_rad(
					json::find_and_convert_member<sge::renderer::scalar>(
						o,
						json::path(FCPPT_TEXT("fov"))))),
			sge::renderer::projection::near(
				json::find_and_convert_member<sge::renderer::scalar>(
					o,
					json::path(FCPPT_TEXT("near")))),
			sge::renderer::projection::far(
				json::find_and_convert_member<sge::renderer::scalar>(
					o,
					json::path(FCPPT_TEXT("far")))));
}
