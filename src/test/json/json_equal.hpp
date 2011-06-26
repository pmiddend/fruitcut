#ifndef FRUITCUT_TEST_JSON_JSON_EQUAL_HPP_INCLUDED
#define FRUITCUT_TEST_JSON_JSON_EQUAL_HPP_INCLUDED

#include <sge/parse/json/json.hpp>

namespace fruitcut
{
namespace test
{
namespace json
{
bool
json_equal(
	sge::parse::json::null const &,
	sge::parse::json::null const &);

bool
json_equal(
	sge::parse::json::object const &,
	sge::parse::json::object const &);

bool
json_equal(
	sge::parse::json::array const &,
	sge::parse::json::array const &);

bool
json_equal(
	fcppt::string const &,
	fcppt::string const &);

bool
json_equal(
	bool,
	bool);

bool
json_equal(
	sge::parse::json::int_type,
	sge::parse::json::int_type);

bool
json_equal(
	sge::parse::json::float_type,
	sge::parse::json::float_type);

bool
json_equal(
	sge::parse::json::value const &,
	sge::parse::json::value const &);

bool
json_equal(
	sge::parse::json::member const &,
	sge::parse::json::member const &);
}
}
}

#endif
