#include <test/json/json_equal.hpp>
#include <test/json/object_from_string.hpp>
#include <sge/parse/json/find_object.hpp>
#include <sge/parse/json/find_object.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/exception.hpp>
#include <sge/parse/json/find_object_exn.hpp>
#include <sge/parse/json/path.hpp>
#include <boost/test/unit_test.hpp>
#include <fcppt/text.hpp>

namespace sgejson = sge::parse::json;

BOOST_AUTO_TEST_CASE(
	json_find_object)
{
	sge::parse::json::object const example_object = 
		fruitcut::test::json::object_from_string(
			FCPPT_TEXT("{ \"foo\" : \"bar\", \"baz\" : { \"qux\" : false,\"quux\" : {} } }"));

	// foo is not an object
	BOOST_CHECK_THROW(
		(sgejson::find_object(
			example_object,
			sgejson::path(
				FCPPT_TEXT("foo")))),
		sge::parse::json::exception);

	BOOST_CHECK_THROW(
		(sgejson::find_object_exn(
			example_object,
			sgejson::path(
				FCPPT_TEXT("foo")))),
		sge::parse::json::exception);

	// baz, however, is
	BOOST_CHECK_NE(
		(sgejson::find_object(
			example_object,
			sgejson::path(
				FCPPT_TEXT("baz")))),
		static_cast<sgejson::object*>(
			0));

	// baz, however, is
	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::find_object_exn(
				example_object,
				sgejson::path(
					FCPPT_TEXT("baz"))),
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{ \"qux\" : false,\"quux\" : {} }")))));

	BOOST_CHECK_NE(
		(sgejson::find_object(
			example_object,
			sgejson::path(
				FCPPT_TEXT("baz"))
				/ FCPPT_TEXT("quux"))),
		static_cast<sgejson::object*>(
			0));
	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::find_object_exn(
				example_object,
				sgejson::path(
					FCPPT_TEXT("baz"))
					/ FCPPT_TEXT("quux")),
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{}")))));

	BOOST_CHECK_EQUAL(
		(sgejson::find_object(
			example_object,
			sgejson::path(
				FCPPT_TEXT("baz"))
				/ FCPPT_TEXT("humbug"))),
		static_cast<sgejson::object*>(
			0));

	BOOST_CHECK_THROW(
		(sgejson::find_object_exn(
			example_object,
			sgejson::path(
				FCPPT_TEXT("baz"))
				/ FCPPT_TEXT("humbug"))),
		sgejson::exception);
}

