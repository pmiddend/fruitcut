#ifndef FRUITCUT_JSON_PARSE_COLOR_HPP_INCLUDED
#define FRUITCUT_JSON_PARSE_COLOR_HPP_INCLUDED

#include <sge/parse/json/value.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/invalid_get.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/exception.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/image/color/init.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/text.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/variant/get.hpp>
#include <vector>

namespace fruitcut
{
namespace json
{
template<typename T>
typename
boost::enable_if_c
<
	boost::is_same<T,sge::image::color::rgba8>::value,
	T const
>::type
parse_color(
	sge::parse::json::value const &v)
{
	try
	{
		sge::parse::json::array const &a = 
			boost::get<sge::parse::json::array>(
				v);

		try
		{
			typedef
			std::vector<sge::parse::json::float_type>
			temp_container;

			temp_container colors;

			BOOST_FOREACH(
				sge::parse::json::value const &elem,
				a.elements)
				colors.push_back(
					boost::get<sge::parse::json::float_type>(
						elem));

			return 
				sge::image::color::rgba8(
					(sge::image::color::init::red %= colors[0])
					(sge::image::color::init::green %= colors[1])
					(sge::image::color::init::blue %= colors[2])
					(sge::image::color::init::alpha %= colors[3]));
		}
		catch (boost::bad_get const &)
		{
			throw 
				sge::exception(
					FCPPT_TEXT("The rgba color I expected didn't contain just floats (maybe you've used an integer?)!"));
		}
	}
	catch (boost::bad_get const &)
	{
		throw 
			sge::exception(
				FCPPT_TEXT("Tried to parse an rgba color, but I have been given no array to work on!"));
	}
}
}
}

#endif
