#ifndef FRUITLIB_JSON_PARSE_PROJECTION_HPP_INCLUDED
#define FRUITLIB_JSON_PARSE_PROJECTION_HPP_INCLUDED

#include <fruitlib/perspective_projection_information.hpp>
#include <fruitlib/detail/symbol.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/optional/object.hpp>


namespace fruitlib
{
namespace json
{
FRUITLIB_DETAIL_SYMBOL
fruitlib::perspective_projection_information const
parse_projection(
	sge::parse::json::object const &,
	fcppt::optional::object<sge::renderer::scalar> const &aspect);
}
}

#endif
