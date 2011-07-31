#include "json_equal.hpp"
#include "object_from_string.hpp"
#include <fruitlib/json/make_recursive_objects.hpp>
#include <fruitlib/json/merge_command_line_parameters.hpp>
#include <fruitlib/json/path.hpp>
#include <sge/parse/json/json.hpp>
#include <boost/test/unit_test.hpp>

namespace sgejson = sge::parse::json;
namespace fruitlibjson = fruitlib::json;

BOOST_AUTO_TEST_CASE(
	json_make_recursive)
{
	sgejson::object current_object = 
		fruitcut::test::json::object_from_string(
			FCPPT_TEXT("{ \"string\" : \"foo\" }"));

	sgejson::object &inner_object =
		fruitlibjson::make_recursive_objects(
			current_object,
			fruitlibjson::path(
				FCPPT_TEXT("foo")));

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			inner_object,
			sgejson::object())));

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			current_object,
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{ \"string\" : \"foo\", \"foo\" : {} }")))));

	sgejson::object &new_inner_object =
		fruitlibjson::make_recursive_objects(
			current_object,
			fruitlibjson::path(
				FCPPT_TEXT("foo"))
				/ FCPPT_TEXT("bar"));

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			current_object,
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{ \"string\" : \"foo\", \"foo\" : { \"bar\" : {} } }")))));

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			inner_object,
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{ \"bar\" : {} }")))));

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			new_inner_object,
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{}")))));

	BOOST_CHECK_THROW((
		fruitlibjson::make_recursive_objects(
			current_object,
			fruitlibjson::path(
				FCPPT_TEXT("string"))
				/ FCPPT_TEXT("bar"))),
		sgejson::exception);
}

BOOST_AUTO_TEST_CASE(
	json_merge_command_line_parameters)
{
	fruitlib::command_line_parameters params;

	params.push_back(
		FCPPT_TEXT("foo/bar=\"baz\""));

	fruitlib::command_line_parameters wrongparams;

	wrongparams.push_back(
		FCPPT_TEXT("foo/bar=3"));

	sgejson::object const current_object = 
		fruitcut::test::json::object_from_string(
			FCPPT_TEXT("{ \"foo\" : { \"bar\" : \"old\" } }"));

	sgejson::object const merged =
		fruitlibjson::merge_command_line_parameters(
			current_object,
			params);

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			merged,
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{ \"foo\" : { \"bar\" : \"baz\" } }")))));

	BOOST_CHECK_THROW((
		fruitlibjson::merge_command_line_parameters(
			current_object,
			wrongparams)),
		sgejson::exception);
}
