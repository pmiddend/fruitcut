#include <fruitlib/json/parse_projection.hpp>
#include <sge/camera/projection/projection.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/variant/object.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/optional.hpp>

sge::camera::projection::object const
fruitlib::json::parse_projection(
	sge::parse::json::object const &o,
	fcppt::optional<sge::renderer::scalar> const &aspect)
{
	if(
		sge::parse::json::find_and_convert_member<fcppt::string>(
			o,
			sge::parse::json::path(FCPPT_TEXT("type"))) == FCPPT_TEXT("orthogonal"))
		return sge::camera::projection::orthogonal();

	return
		sge::camera::projection::perspective(
			sge::renderer::projection::aspect(
				aspect
				?
					*aspect
				:
					sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
						o,
						sge::parse::json::path(FCPPT_TEXT("aspect")))),
			sge::renderer::projection::fov(
				fcppt::math::deg_to_rad(
					sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
						o,
						sge::parse::json::path(FCPPT_TEXT("fov"))))),
			sge::renderer::projection::near(
				sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
					o,
					sge::parse::json::path(FCPPT_TEXT("near")))),
			sge::renderer::projection::far(
				sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
					o,
					sge::parse::json::path(FCPPT_TEXT("far")))));
}
