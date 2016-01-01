#include <fruitlib/exception.hpp>
#include <fruitlib/json/parse_projection.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/optional/from.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/variant/object.hpp>


fruitlib::perspective_projection_information const
fruitlib::json::parse_projection(
	sge::parse::json::object const &o,
	fcppt::optional::object<sge::renderer::scalar> const &aspect)
{
	return
		fruitlib::perspective_projection_information(
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
					sge::parse::json::path(FCPPT_TEXT("far")))),
			sge::renderer::projection::aspect(
				fcppt::optional::from(
					aspect,
					[
						&o
					]{
						return
							sge::parse::json::find_and_convert_member<sge::renderer::scalar>(
								o,
								sge::parse::json::path(FCPPT_TEXT("aspect"))
							);
					}
				)
			)
		);
}
