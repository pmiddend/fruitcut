#ifndef FRUITLIB_JSON_FIND_AND_CONVERT_MEMBER_HPP_INCLUDED
#define FRUITLIB_JSON_FIND_AND_CONVERT_MEMBER_HPP_INCLUDED

#include "path.hpp"
#include "convert_from.hpp"
#include "is_null.hpp"
#include "path_to_string.hpp"
#include "find_object_exn.hpp"
#include "../exception.hpp"
#include <sge/parse/json/json.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <boost/range/algorithm/find_if.hpp>

namespace fruitlib
{
namespace json
{
template<typename T>
T const 
find_and_convert_member(
	sge::parse::json::object const &o,
	json::path const &input_path)
{
	FCPPT_ASSERT(
		!input_path.empty());

	json::path const shortened_path = 
		json::path(
			input_path.begin(),
			--input_path.end());

	sge::parse::json::object const &found_object = 
		json::find_object_exn(
			o,
			shortened_path);

	sge::parse::json::member_vector::const_iterator it = 
		boost::find_if(
			found_object.members,
			sge::parse::json::member_name_equal(
				input_path.back()));

	if (it == found_object.members.end())
		throw 
			sge::parse::json::exception(
				FCPPT_TEXT("Couldn't find member \"")+
				input_path.back()+
				FCPPT_TEXT("\" in the object \"")+
				json::path_to_string(
					shortened_path)+
				FCPPT_TEXT("\""));

	if(
		json::is_null(
			it->value))
		throw 
			sge::parse::json::exception(
				FCPPT_TEXT("The member \"")+
				it->name+
				FCPPT_TEXT("\" of object \"")+
				json::path_to_string(
					shortened_path)+
				FCPPT_TEXT("\" is null. Don't know what to return here..."));

	try
	{
		return 
			json::convert_from<T>(
				it->value);
	}
	catch (sge::parse::json::invalid_get const &e)
	{
		throw 
			sge::parse::json::exception(
				FCPPT_TEXT("Unable to parse member \"")+
				it->name+
				FCPPT_TEXT(" of object \"")+
				json::path_to_string(
					shortened_path)+
				FCPPT_TEXT("\": ")+
				e.string());
	}
}
}
}

#endif
