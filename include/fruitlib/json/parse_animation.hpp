#ifndef FRUITLIB_JSON_PARSE_ANIMATION_HPP_INCLUDED
#define FRUITLIB_JSON_PARSE_ANIMATION_HPP_INCLUDED

#include <fruitlib/exception.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/invalid_get.hpp>
#include <sge/parse/json/value.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/variant/get.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace json
{
template
<
	typename Animation,
	typename ValueParser
>
typename
Animation::keyframe_sequence const
parse_animation(
	sge::parse::json::array const &a,
	ValueParser const &parse_value)
{
	typedef typename
	Animation::value_sequence
	result_type;

	typedef typename
	Animation::duration
	duration;

	typedef typename
	Animation::value_type
	value_type;

	typedef typename
	Animation::keyframe
	keyframe_type;

	result_type keyframes;

	for(
		sge::parse::json::element_vector::const_iterator v =
			a.elements.begin();
		v != a.elements.end();
		++v)
	{
		try
		{
			sge::parse::json::object const &o =
				boost::get<sge::parse::json::object>(
					*v);

			keyframes.push_back(
				keyframe_type(
					parse_value(
						json::find_and_convert_member<sge::parse::json::value>(
							o,
							json::path(
								FCPPT_TEXT("color")))),
					time_format::find_and_convert_duration<duration>(
						o,
						json::path(
							FCPPT_TEXT("time")))));
		}
		catch (boost::bad_get const &)
		{
			throw
				fruitlib::exception(
					FCPPT_TEXT("An animation should consist of a list of objects. This one doesn't!"));
		}
	}

	return
		keyframes;
}
}
}

#endif
