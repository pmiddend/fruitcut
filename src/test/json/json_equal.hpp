#ifndef FRUITCUT_TEST_JSON_JSON_EQUAL_HPP_INCLUDED
#define FRUITCUT_TEST_JSON_JSON_EQUAL_HPP_INCLUDED

#include <sge/parse/json/null.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/array_fwd.hpp>
#include <sge/parse/json/int_type.hpp>
#include <sge/parse/json/member_fwd.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/value.hpp>
#include <fcppt/string.hpp>

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
