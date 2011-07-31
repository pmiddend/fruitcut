#ifndef FRUITLIB_JSON_CONVERT_TO_HPP_INCLUDED
#define FRUITLIB_JSON_CONVERT_TO_HPP_INCLUDED

#include <sge/parse/json/json.hpp>
#include <fcppt/type_traits/is_iterable.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_floating_point.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <fcppt/string.hpp>

namespace fruitlib
{
namespace json
{
template<typename T>
typename
boost::enable_if_c
<
	boost::is_same<T,sge::parse::json::object>::value ||
	boost::is_same<T,sge::parse::json::array>::value ||
	boost::is_same<T,sge::parse::json::null>::value || 
	boost::is_same<T,fcppt::string>::value || 
	boost::is_same<T,bool>::value,
	sge::parse::json::value
>::type
convert_to(
	T const &t)
{
	return 
		t;
}

template<typename T>
typename
boost::enable_if_c
<
	boost::is_integral<T>::value &&
	!boost::is_same<T,bool>::value,
	sge::parse::json::value
>::type
convert_to(
	T const &t)
{
	return 
		static_cast<sge::parse::json::int_type>(
			t);
}

template<typename T>
typename
boost::enable_if_c
<
	boost::is_floating_point<T>::value,
	sge::parse::json::value
>::type
convert_to(
	T const &t)
{
	return 
		static_cast<sge::parse::json::float_type>(
			t);
}

// TODO: We could make a convert_to which converts from a tuple to a
// (heterogenous) array
template<typename T>
typename
boost::enable_if_c
<
	fcppt::type_traits::is_iterable<T>::value && 
	!boost::is_same<T,fcppt::string>::value,
	sge::parse::json::value
>::type
convert_to(
	T const &t)
{
	sge::parse::json::array result;
	for(typename T::const_iterator i = t.begin(); i != t.end(); ++i)
		result.elements.push_back(
			json::convert_to(
				*i));
	return result;
}
}
}

#endif
