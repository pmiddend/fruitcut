#ifndef FRUITCUT_TEST_JSON_OBJECT_FROM_STRING_HPP_INCLUDED
#define FRUITCUT_TEST_JSON_OBJECT_FROM_STRING_HPP_INCLUDED

#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/string.hpp>


namespace fruitcut
{
namespace test
{
namespace json
{
sge::parse::json::object const
object_from_string(
	fcppt::string const &);
}
}
}

#endif
