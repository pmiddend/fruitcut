#ifndef FRUITCUT_JSON_FIND_MEMBER_HPP_INCLUDED
#define FRUITCUT_JSON_FIND_MEMBER_HPP_INCLUDED

#include "convert.hpp"
#include "member_has_type.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/member_name_equal.hpp>
#include <sge/exception.hpp>
#include <fcppt/string.hpp>
#include <fcppt/algorithm/std/accumulate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include <algorithm>

#include <boost/foreach.hpp>
#include <fcppt/io/cout.hpp>
namespace fruitcut
{
namespace json
{
template<typename T>
T const 
find_member(
	sge::parse::json::object const &o,
	fcppt::string const &path)
{
	typedef
	std::vector<fcppt::string>
	string_vector;

	string_vector parts;

	boost::algorithm::split(
		parts,
		path,
		boost::algorithm::is_any_of(FCPPT_TEXT("/")));

	fcppt::string const last_element = parts.back();

	parts.pop_back();

	sge::parse::json::object const *target = 
		fcppt::algorithm::std::accumulate(
			parts,
			&o,
			[](
				sge::parse::json::object const *o,
				fcppt::string const &s)
			{
				return 
					&sge::parse::json::find_member_exn<sge::parse::json::object>(
						o->members,
						s);
			});

	sge::parse::json::member_vector::const_iterator it = 
		std::find_if(
			target->members.begin(),
			target->members.end(),
			sge::parse::json::member_name_equal(
				last_element));

	if (it == target->members.end())
		throw sge::exception(
			FCPPT_TEXT("Couldn't find member \"")+last_element+FCPPT_TEXT("\""));

	try
	{
		sge::parse::json::get<sge::parse::json::null>(
			it->value);
		throw sge::exception(FCPPT_TEXT("The member \"")+path+FCPPT_TEXT("\" is null"));
	}
	catch (sge::parse::json::invalid_get const &)
	{
		try
		{
			return 
				fruitcut::json::convert<T>(
					it->value);
		}
		catch (sge::parse::json::invalid_get const &e)
		{
			throw sge::exception(
				FCPPT_TEXT("Unable to parse \"")+
				it->name+
				FCPPT_TEXT("\": ")+
				e.string());
		}
	}

}
}
}

#endif
