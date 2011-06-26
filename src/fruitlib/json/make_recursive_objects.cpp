#include "make_recursive_objects.hpp"
#include "path_to_string.hpp"
#include "path.hpp"
#include <sge/parse/json/json.hpp>
#include <boost/variant/get.hpp>
#include <fcppt/string.hpp>
#include <fcppt/type_name.hpp>
#include <fcppt/text.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/assert.hpp>
#include <numeric>
#include <algorithm>

namespace
{
sge::parse::json::object *
create_or_navigate_path(
	fruitcut::fruitlib::json::path const &input_path,
	sge::parse::json::object * const old,
	fcppt::string const &new_member)
{
	FCPPT_ASSERT(
		old);

	sge::parse::json::member_vector::iterator it = 
		std::find_if(
			old->members.begin(),
			old->members.end(),
			sge::parse::json::member_name_equal(
				new_member));

	if(it == old->members.end())
	{
		old->members.push_back(
			sge::parse::json::member(
				new_member,
				sge::parse::json::object()));

		return &boost::get<sge::parse::json::object>(old->members.back().value);
	}

	if(it->value.type() != typeid(sge::parse::json::object))
		throw 
			sge::parse::json::exception(
				FCPPT_TEXT("Couldn't navigate to (make_recursive) \"")+
				fruitcut::fruitlib::json::path_to_string(
					input_path)+
				FCPPT_TEXT("\", stopped at \"")+
				new_member+
				FCPPT_TEXT("\" because this member has type \"")+
				fcppt::type_name(
					it->value.type())+
				FCPPT_TEXT("\" instead of type sge::parse::json::object!"));

	return 
		&boost::get<sge::parse::json::object>(
			it->value);
}
}

sge::parse::json::object &
fruitcut::fruitlib::json::make_recursive_objects(
	sge::parse::json::object &input_object,
	json::path const &input_path)
{
	return 
		*std::accumulate(
			input_path.begin(),
			input_path.end(),
			&input_object,
			std::tr1::bind(
				&create_or_navigate_path,
				input_path,
				std::tr1::placeholders::_1,
				std::tr1::placeholders::_2));
}
