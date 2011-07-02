#include <boost/test/unit_test.hpp>
#include <sge/parse/json/json.hpp>
#include <fcppt/string.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <fcppt/strong_typedef.hpp>
#include "../../fruitlib/json/convert_from.hpp"

namespace sgejson = sge::parse::json;
namespace fruitlibjson = fruitcut::fruitlib::json;

namespace
{
// We have to define it so we can define the operator<< (which
// boost.test needs)
FCPPT_MAKE_STRONG_TYPEDEF(
	std::vector < std::vector<int> >,
	vector_2d);

template<typename Char,typename CharTraits>
std::basic_ostream<Char,CharTraits> &
operator<<(
	std::basic_ostream<Char,CharTraits> &s,
	vector_2d const &v)
{
	typedef
	vector_2d::value_type
	container;

	s << s.widen('[');

	for(container::const_iterator i = v.get().begin(); i != v.get().end(); ++i)
	{
		s << s.widen('[');

		for(container::value_type::const_iterator j = i->begin(); j != i->end(); ++j)
		{
			s << *j;
			if(j != --i->end())
				s << s.widen(',');
		}
		
		s << s.widen(']');
		if(i != --v.get().end())
			s << s.widen(',');
	}
		
	
	s << s.widen(']');
	return s;
}
}

BOOST_AUTO_TEST_CASE(
	json_convert_from_float)
{
	// Test if floating point values are converted
	float const float_lit = 1.0f;
	double const double_lit = 1.0;
	long double const long_double_lit = 1.0L;

	float const float_eps = 0.0001f;
	double const double_eps = 0.0001;
	long double long_double_eps = 0.0001L;

	// Check if conversion to the floating point types is ok
	BOOST_CHECK_CLOSE(
		fruitlibjson::convert_from<float>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					float_lit))),
		float_lit,
		float_eps);

	BOOST_CHECK_CLOSE(
		fruitlibjson::convert_from<double>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					double_lit))),
		double_lit,
		double_eps);

	BOOST_CHECK_CLOSE(
		fruitlibjson::convert_from<long double>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					long_double_lit))),
		long_double_lit,
		long_double_eps);

	// Check if conversion to the NON floating point types is NOT ok and
	// results in a sgejson::exception
	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<int>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					1.0f))),
		sgejson::exception);

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<char>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					1.0f))),
		sgejson::exception);

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<unsigned int>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					1.0f))),
		sgejson::exception);

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<short>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					1.0f))),
		sgejson::exception);

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<void*>(
			sgejson::value(
				static_cast<sgejson::float_type>(
					1.0f))),
		sgejson::exception);
}

BOOST_AUTO_TEST_CASE(
	json_convert_from_bool)
{
	BOOST_CHECK_EQUAL(
		fruitlibjson::convert_from<bool>(
			sgejson::value(
				true)),
		true);

	BOOST_CHECK_EQUAL(
		fruitlibjson::convert_from<bool>(
			sgejson::value(
				false)),
		false);

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<bool>(
			sgejson::value(
				static_cast<sgejson::int_type>(
					1))),
		sgejson::exception);
}

BOOST_AUTO_TEST_CASE(
	json_convert_from_integer)
{
	BOOST_CHECK_EQUAL(
		fruitlibjson::convert_from<int>(
			sgejson::value(
				static_cast<sgejson::int_type>(
					1))),
		1);

	BOOST_CHECK_EQUAL(
		fruitlibjson::convert_from<unsigned>(
			sgejson::value(
				static_cast<sgejson::int_type>(
					1))),
		1u);

	BOOST_CHECK_EQUAL(
		fruitlibjson::convert_from<char>(
			sgejson::value(
				static_cast<sgejson::int_type>(
					1))),
		static_cast<char>(
			1));

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<int>(
			sgejson::value(
				fcppt::string(FCPPT_TEXT("foo")))),
		sgejson::exception);
}

BOOST_AUTO_TEST_CASE(
	json_convert_from_matrix)
{
	typedef
	fcppt::math::matrix::static_<sgejson::int_type,3,4>::type
	matrix_type;

	matrix_type const test_matrix(
		1,2,3, 
		4,5,6,
		7,8,9,
		1,2,3); 

	// Wrong integer type, wrong dimensions (one dimensional)
	sgejson::array wrong_integer_wrong_dimensions;
	wrong_integer_wrong_dimensions.elements.push_back(
		static_cast<sgejson::float_type>(1));
	wrong_integer_wrong_dimensions.elements.push_back(
		static_cast<sgejson::int_type>(1));
	wrong_integer_wrong_dimensions.elements.push_back(
		fcppt::string(
			FCPPT_TEXT("haha")));

	// Correct internal type, wrong dimensions (one dimensional)
	sgejson::array right_integer_wrong_dimensions;
	right_integer_wrong_dimensions.elements.push_back(
		static_cast<sgejson::int_type>(
			1));
	right_integer_wrong_dimensions.elements.push_back(
		static_cast<sgejson::int_type>(
			2));
	right_integer_wrong_dimensions.elements.push_back(
		static_cast<sgejson::int_type>(
			3));

	// Correct internal type, correct dimensions
	sgejson::array right_integer_right_dimensions;
	right_integer_right_dimensions.elements.resize(4);
	sgejson::array temp_array;
	temp_array.elements.resize(3);
	temp_array.elements[0] = 1;
	temp_array.elements[1] = 2;
	temp_array.elements[2] = 3;
	right_integer_right_dimensions.elements[0] = temp_array;
	temp_array.elements[0] = 4;
	temp_array.elements[1] = 5;
	temp_array.elements[2] = 6;
	right_integer_right_dimensions.elements[1] = temp_array;
	temp_array.elements[0] = 7;
	temp_array.elements[1] = 8;
	temp_array.elements[2] = 9;
	right_integer_right_dimensions.elements[2] = temp_array;
	temp_array.elements[0] = 1;
	temp_array.elements[1] = 2;
	temp_array.elements[2] = 3;
	right_integer_right_dimensions.elements[3] = temp_array;

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<matrix_type>(
			wrong_integer_wrong_dimensions),
		sgejson::exception);

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<matrix_type>(
			right_integer_wrong_dimensions),
		sgejson::exception);

	BOOST_CHECK_EQUAL(
		fruitlibjson::convert_from<matrix_type>(
			right_integer_right_dimensions),
		test_matrix);
}

BOOST_AUTO_TEST_CASE(
	json_convert_from_iterable)
{
	sgejson::array homogenous_input;
	homogenous_input.elements.push_back(
		static_cast<sgejson::int_type>(
			10));
	homogenous_input.elements.push_back(
		static_cast<sgejson::int_type>(
			20));
	homogenous_input.elements.push_back(
		static_cast<sgejson::int_type>(
			30));

	typedef
	fcppt::math::vector::static_<int,3>::type
	math_vector_type;

	BOOST_CHECK_EQUAL(
		fruitlibjson::convert_from<math_vector_type>(
			homogenous_input),
		math_vector_type(10,20,30));

	typedef
	std::vector<int>
	vector;

	vector real_vector;
	real_vector.push_back(
		10);
	real_vector.push_back(
		20);
	real_vector.push_back(
		30);

	BOOST_CHECK(
		fruitlibjson::convert_from<vector>(
			homogenous_input) == real_vector);

	typedef
	fcppt::container::array<int,3>
	array;

	array const array_ = 
		{{ 10, 20, 30 }};

	BOOST_CHECK(
		fruitlibjson::convert_from<array>(
			homogenous_input) == array_);

	// It has 4 elements, it's fucking huge!
	typedef
	fcppt::container::array<int,4>
	huge_array;

	huge_array const huge_array_ = 
		{{ 10, 20, 30, 40 }};

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<huge_array>(
			homogenous_input) == huge_array_,
		sgejson::exception);

	// Can we go two-dimensional?
	sgejson::array input_2d;
	sgejson::array subarray;
	subarray.elements.push_back(
		static_cast<sgejson::int_type>(
			10));
	subarray.elements.push_back(
		static_cast<sgejson::int_type>(
			20));
	subarray.elements.push_back(
		static_cast<sgejson::int_type>(
			30));
	input_2d.elements.push_back(
		subarray);
	subarray.elements[0] = 
		static_cast<sgejson::int_type>(
			1);
	subarray.elements[1] = 
		static_cast<sgejson::int_type>(
			2);
	subarray.elements[2] = 
		static_cast<sgejson::int_type>(
			3);
	input_2d.elements.push_back(
		subarray);

	vector_2d const expected_2d_output(
		fcppt::assign::make_container<vector_2d::value_type>
			(fcppt::assign::make_container<vector_2d::value_type::value_type>(10)(20)(30))
			(fcppt::assign::make_container<vector_2d::value_type::value_type>(1)(2)(3)));

	BOOST_CHECK_EQUAL(
		vector_2d(
			fruitlibjson::convert_from<vector_2d::value_type>(
				input_2d)),
		expected_2d_output);

	sgejson::array heterogenous_input;
	heterogenous_input.elements.push_back(
		static_cast<sgejson::int_type>(
			10));
	heterogenous_input.elements.push_back(
		fcppt::string(
			FCPPT_TEXT("foo")));
	heterogenous_input.elements.push_back(
		fcppt::string(
			FCPPT_TEXT("bar")));

	BOOST_CHECK_THROW(
		fruitlibjson::convert_from<array>(
			heterogenous_input),
		sgejson::exception);
}

BOOST_AUTO_TEST_CASE(
	json_convert_from_string)
{
	// Can't use BOOST_CHECK_EQUAL here since fcppt::string doesn't have
	// an operator<< for std::cout (boost.test is broken?)
	BOOST_CHECK(
		fruitlibjson::convert_from<fcppt::string>(
			fcppt::string(
				FCPPT_TEXT("foobar"))) == 
		fcppt::string(
			FCPPT_TEXT("foobar")));
}

