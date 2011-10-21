#include "json_equal.hpp"
#include "object_from_string.hpp"
#include <sge/parse/json/exception.hpp>
#include <sge/parse/json/make_recursive_objects.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/parse/json/config/merge_command_line_parameters.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/test/unit_test.hpp>
#include <fcppt/config/external_end.hpp>


namespace sgejson = sge::parse::json;

BOOST_AUTO_TEST_CASE(
	json_make_recursive)
{
	sgejson::object current_object =
		fruitcut::test::json::object_from_string(
			FCPPT_TEXT("{ \"string\" : \"foo\" }"));

	sgejson::object &inner_object =
		sgejson::make_recursive_objects(
			current_object,
			sgejson::path(
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
		sgejson::make_recursive_objects(
			current_object,
			sgejson::path(
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
		sgejson::make_recursive_objects(
			current_object,
			sgejson::path(
				FCPPT_TEXT("string"))
				/ FCPPT_TEXT("bar"))),
		sgejson::exception);
}

BOOST_AUTO_TEST_CASE(
	json_merge_command_line_parameters)
{
	sgejson::config::command_line_parameters params;

	params.push_back(
		FCPPT_TEXT("foo/bar=\"baz\""));

	sgejson::config::command_line_parameters wrongparams;

	wrongparams.push_back(
		FCPPT_TEXT("foo/bar=3"));

	sgejson::object const current_object =
		fruitcut::test::json::object_from_string(
			FCPPT_TEXT("{ \"foo\" : { \"bar\" : \"old\" } }"));

	sgejson::object const merged =
		sgejson::config::merge_command_line_parameters(
			current_object,
			params);

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			merged,
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{ \"foo\" : { \"bar\" : \"baz\" } }")))));

	BOOST_CHECK_THROW((
		sgejson::config::merge_command_line_parameters(
			current_object,
			wrongparams)),
		sgejson::exception);
}
