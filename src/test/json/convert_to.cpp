#include "json_equal.hpp"
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/convert_to.hpp>
#include <sge/parse/json/int_type.hpp>
#include <sge/parse/json/null.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/test/unit_test.hpp>
#include <typeinfo>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace sgejson = sge::parse::json;

BOOST_AUTO_TEST_CASE(
	json_convert_to)
{
	// Test if, for json types, convert_to is the identity

	// String
	fcppt::string input_string(
		FCPPT_TEXT("foobar"));

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				input_string),
			sgejson::value(
				input_string))));

	bool input_bool = false;

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				input_bool),
			sgejson::value(
				input_bool))));

	sgejson::float_type input_float =
		static_cast<sgejson::float_type>(1.0);

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				input_float),
			sgejson::value(
				input_float))));

	sgejson::int_type input_int =
		static_cast<sgejson::int_type>(1.0);

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				input_int),
			sgejson::value(
				input_int))));

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				sgejson::null()),
			sgejson::value(
				sgejson::null()))));

	std::vector<unsigned> input_array;
	input_array.push_back(1);
	input_array.push_back(2);
	input_array.push_back(3);

	sgejson::array expected_array;
	expected_array.elements.push_back(
		static_cast<sgejson::int_type>(
			1));
	expected_array.elements.push_back(
		static_cast<sgejson::int_type>(
			2));
	expected_array.elements.push_back(
		static_cast<sgejson::int_type>(
			3));

	// require because the test below uses input_array again
	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				input_array),
			sgejson::value(
				expected_array))));

	// The same with fixed arrays
	fcppt::container::array<unsigned,3> const real_array =
		{{1,2,3}};

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				real_array),
			sgejson::value(
				expected_array))));

	// Can we go two-dimensional?
	fcppt::container::array<fcppt::container::array<unsigned,2>,3> two_dim_array =
		{{
			{{1,2}},
			{{3,4}},
			{{5,6}}
		}};

	sgejson::array expected_2d_array;
	sgejson::array inner_2d_array;
	inner_2d_array.elements.push_back(
		static_cast<sge::parse::json::int_type>(
			1));
	inner_2d_array.elements.push_back(
		static_cast<sge::parse::json::int_type>(
			2));
	expected_2d_array.elements.push_back(
		inner_2d_array);
	inner_2d_array.elements[0] =
		static_cast<sge::parse::json::int_type>(
			3);
	inner_2d_array.elements[1] =
		static_cast<sge::parse::json::int_type>(
			4);
	expected_2d_array.elements.push_back(
		inner_2d_array);
	inner_2d_array.elements[0] =
		static_cast<sge::parse::json::int_type>(
			5);
	inner_2d_array.elements[1] =
		static_cast<sge::parse::json::int_type>(
			6);
	expected_2d_array.elements.push_back(
		inner_2d_array);

	BOOST_CHECK((
		fruitcut::test::json::json_equal(
			sgejson::convert_to(
				two_dim_array),
			sgejson::value(
				expected_2d_array))));
}
