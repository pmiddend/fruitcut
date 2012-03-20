#ifndef FRUITLIB_JSON_PARSE_PROJECTION_HPP_INCLUDED
#define FRUITLIB_JSON_PARSE_PROJECTION_HPP_INCLUDED

#include <sge/camera/projection/object.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/optional.hpp>

namespace fruitlib
{
namespace json
{
sge::camera::projection::object const
parse_projection(
	sge::parse::json::object const &,
	fcppt::optional<sge::renderer::scalar> const &aspect);
}
}

#endif
