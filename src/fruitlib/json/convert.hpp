#ifndef FRUITCUT_FRUITLIB_JSON_CONVERT_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_CONVERT_HPP_INCLUDED

#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/type_traits/is_iterable.hpp>
#include <fcppt/string.hpp>
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
		&& !boost::is_same<T,sge::parse::json::string>::value,
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
