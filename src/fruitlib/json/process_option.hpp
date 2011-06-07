#ifndef FRUITCUT_FRUITLIB_JSON_PROCESS_OPTION_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_PROCESS_OPTION_HPP_INCLUDED

#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
void
process_option(
	sge::parse::json::object &,
	fcppt::string const &);
}
}
}

#endif