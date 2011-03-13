#ifndef FRUITCUT_JSON_PARSE_COLOR_HPP_INCLUDED
#define FRUITCUT_JSON_PARSE_COLOR_HPP_INCLUDED

#include "parse_rgba8_color.hpp"
#include <sge/image/color/convert.hpp>
#include <sge/parse/json/value.hpp>

namespace fruitcut
{
namespace json
{
template<typename Color>
Color const
parse_color(
	sge::parse::json::value const &v)
{
	return 
		sge::image::color::convert<typename Color::format>(
			parse_rgba8_color(
				v));
}
}
}

#endif
