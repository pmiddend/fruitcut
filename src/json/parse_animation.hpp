#ifndef FRUITCUT_JSON_PARSE_ANIMATION_HPP_INCLUDED
#define FRUITCUT_JSON_PARSE_ANIMATION_HPP_INCLUDED

#include "find_member.hpp"
#include "../animation.hpp"
#include "../exception.hpp"
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/invalid_get.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/time/second_f.hpp>
#include <sge/time/funit.hpp>
#include <boost/variant/get.hpp>
#include <fcppt/text.hpp>
#include <boost/foreach.hpp>

namespace fruitcut
{
namespace json
{
template<typename Animation,typename ValueParser>
typename 
Animation::value_sequence const
parse_animation(
	sge::parse::json::array const &a,
	ValueParser const &parse_value)
{
	typedef typename 
	Animation::value_sequence
	result_type;

	typedef typename
	result_type::value_type
	value_type;

	result_type values;

	BOOST_FOREACH(
		sge::parse::json::value const &v,
		a.elements)
	{
		try
		{
			sge::parse::json::object const &o = 
				boost::get<sge::parse::json::object>(
					v);

			sge::parse::json::float_type time_seconds = 
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
								o.members,
								FCPPT_TEXT("time"));
			sge::parse::json::value const &colorvalue = 
				fruitcut::json::find_member<sge::parse::json::value>(
						o,
						FCPPT_TEXT("color"));

			values.push_back(
				value_type(
					sge::time::second_f(
						static_cast<sge::time::funit>(
							time_seconds)),
				parse_value(
					colorvalue)));
		}
		catch (boost::bad_get const &)
		{
			throw 
				exception(
					FCPPT_TEXT("An animation should consist of a list of objects. This one doesn't!"));
		}
	}

	return values;
}
}
}

#endif
