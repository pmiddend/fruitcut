#include "json_equal.hpp"
#include "object_from_string.hpp"
#include "../../fruitlib/json/find_object.hpp"
#include "../../fruitlib/json/find_object_exn.hpp"
#include "../../fruitlib/json/path.hpp"
#include <sge/parse/json/json.hpp>
#include <boost/test/unit_test.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>

namespace sgejson = sge::parse::json;
namespace fruitlibjson = fruitcut::fruitlib::json;

BOOST_AUTO_TEST_CASE(
	json_find_object)
{
	sge::parse::json::object const example_object = 
		fruitcut::test::json::object_from_string(
			FCPPT_TEXT("{ \"foo\" : \"bar\", \"baz\" : { \"qux\" : false,\"quux\" : {} } }"));

	// foo is not an object
	BOOST_CHECK_THROW(
		(fruitlibjson::find_object(
			example_object,
			fruitlibjson::path(
				FCPPT_TEXT("foo")))),
		sge::parse::json::exception);

	BOOST_CHECK_THROW(
		(fruitlibjson::find_object_exn(
			example_object,
			fruitlibjson::path(
				FCPPT_TEXT("foo")))),
		sge::parse::json::exception);

	// baz, however, is
	BOOST_CHECK_NE(
		(fruitlibjson::find_object(
			example_object,
			fruitlibjson::path(
				FCPPT_TEXT("baz")))),
		static_cast<sgejson::object*>(
			0));

	// baz, however, is
	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			fruitlibjson::find_object_exn(
				example_object,
				fruitlibjson::path(
					FCPPT_TEXT("baz"))),
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{ \"qux\" : false,\"quux\" : {} }")))));

	BOOST_CHECK_NE(
		(fruitlibjson::find_object(
			example_object,
			fruitlibjson::path(
				FCPPT_TEXT("baz"))
				/ FCPPT_TEXT("quux"))),
		static_cast<sgejson::object*>(
			0));
	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			fruitlibjson::find_object_exn(
				example_object,
				fruitlibjson::path(
					FCPPT_TEXT("baz"))
					/ FCPPT_TEXT("quux")),
			fruitcut::test::json::object_from_string(
				FCPPT_TEXT("{}")))));

	BOOST_CHECK_EQUAL(
		(fruitlibjson::find_object(
			example_object,
			fruitlibjson::path(
				FCPPT_TEXT("baz"))
				/ FCPPT_TEXT("humbug"))),
		static_cast<sgejson::object*>(
			0));

	BOOST_CHECK_THROW(
		(fruitlibjson::find_object_exn(
			example_object,
			fruitlibjson::path(
				FCPPT_TEXT("baz"))
				/ FCPPT_TEXT("humbug"))),
		sgejson::exception);
}

