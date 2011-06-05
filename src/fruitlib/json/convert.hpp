#ifndef FRUITCUT_FRUITLIB_JSON_CONVERT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_CONVERT_HPP_INCLUDED

#include "../exception.hpp"
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/type_traits/is_iterable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/algorithm/map.hpp>
#include <boost/mpl/and.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{

template<typename T>
T const
apply_convert(
	sge::parse::json::value const &_value
);

// Assume float_type
template<typename T>
typename
boost::enable_if_c
<
	boost::is_floating_point<T>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		static_cast<T>(
			sge::parse::json::get<sge::parse::json::float_type>(
				v));
}

// Assume float_type
template<typename T>
typename
boost::enable_if_c
<
	fcppt::math::matrix::is_matrix<T>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	sge::parse::json::element_vector const 
		outer_array = 
			sge::parse::json::get<sge::parse::json::array>(
				v).elements,
		first_outer_vector = 
			sge::parse::json::get<sge::parse::json::array>(
				outer_array.front()).elements;

	T result(
		typename T::dim(
			static_cast<typename T::size_type>(
				outer_array.size()),
			static_cast<typename T::size_type>(
				first_outer_vector.size())));

	for(
		sge::parse::json::element_vector::size_type i = 0;
		i != outer_array.size();
		++i)
	{
		sge::parse::json::element_vector const current_inner_array = 
			sge::parse::json::get<sge::parse::json::array>(
				outer_array[i]).elements;

		if(current_inner_array.size() != first_outer_vector.size())
			throw fruitlib::exception(FCPPT_TEXT("You tried to input a matrix, but the inner dimensions don't match!"));

		for(
			sge::parse::json::element_vector::size_type j = 0;
			j != current_inner_array.size();
			++j)
		{
			result[static_cast<typename T::size_type>(i)][static_cast<typename T::size_type>(j)] = 
				json::convert<typename T::value_type>(
					current_inner_array[j]);
		}
	}

	return result;
}

// Assume int_type
template<typename T>
typename
boost::enable_if_c
<
	boost::is_integral<T>::value && !boost::is_same<T,bool>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		static_cast<T>(
			sge::parse::json::get<sge::parse::json::int_type>(
				v));
}

// Assume bool
template<typename T>
typename
boost::enable_if_c
<
	boost::is_same<T,bool>::value,
	bool
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		sge::parse::json::get<bool>(v);
}

// Assume array_type (exception string)
template<typename T>
typename
boost::enable_if_c
<
	fcppt::type_traits::is_iterable<T>::value 
		&& !boost::is_same<T,sge::parse::json::string>::value
		&& !fcppt::math::matrix::is_matrix<T>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	// This seems like a detour (first converting to vector and then to
	// the target container T) but this ensures generality: You can
	// "reach" more containers when you only demand iterator
	// initialization (and the existance of ::value_type)
	std::vector<typename T::value_type> const temp = 
		fcppt::algorithm::map<std::vector<typename T::value_type> >(
			sge::parse::json::get<sge::parse::json::array>(
				v).elements,
			&json::apply_convert<typename T::value_type>);

	return T(temp.begin(),temp.end());
}

// This special case handles fcppt::string which would otherwise be
// seen as an array
template<typename T>
typename
boost::enable_if_c
<
	boost::is_same<T,sge::parse::json::string>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		sge::parse::json::get<sge::parse::json::string>(
			v);
}

// Assume...nothing. You cannot json::get for a json::value, hence
// this special case
template<typename T>
typename
boost::enable_if_c
<
	boost::is_same<T,sge::parse::json::value>::value,
	sge::parse::json::value const
>::type
convert(
	sge::parse::json::value const &v)
{
	return v;
}

// Everything else -> has to be a json type!
template<typename T>
typename
boost::enable_if_c
<
	!boost::is_floating_point<T>::value 
		&& !boost::is_integral<T>::value
		&& !fcppt::type_traits::is_iterable<T>::value
		&& !boost::is_same<T,sge::parse::json::value>::value,
	T const
>::type
convert(
	sge::parse::json::value const &v)
{
	return 
		sge::parse::json::get<T>(
			v);
}

template<typename T>
T const
apply_convert(
	sge::parse::json::value const &_value
)
{
	return json::convert<T>(_value);
}

}
}
}

#endif
