#include "parse_projection.hpp"
#include "find_member.hpp"
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
		json::find_member<fcppt::string>(
			o,
			FCPPT_TEXT("type")) == FCPPT_TEXT("orthogonal"))
		return sge::camera::projection::orthogonal();

	return 
		sge::camera::projection::perspective(
			aspect
			?
				*aspect
			:
				json::find_member<sge::renderer::scalar>(
					o,
					FCPPT_TEXT("aspect")),
			fcppt::math::deg_to_rad(
				json::find_member<sge::renderer::scalar>(
					o,
					FCPPT_TEXT("fov"))),
			json::find_member<sge::renderer::scalar>(
				o,
				FCPPT_TEXT("near")),
			json::find_member<sge::renderer::scalar>(
				o,
				FCPPT_TEXT("far")));
}
