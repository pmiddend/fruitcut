#include "find_object_exn.hpp"
#include "path_to_string.hpp"
#include "path.hpp"
#include "../exception.hpp"
#include <sge/parse/json/json.hpp>
#include <fcppt/type_name.hpp>
#include <fcppt/text.hpp>
#include <boost/variant/get.hpp>
#include <algorithm>
#include <typeinfo>

sge::parse::json::object const &
fruitcut::fruitlib::json::find_object_exn(
	sge::parse::json::object const &input_object,
	json::path const &p)
{
	sge::parse::json::object const *current_object = 
		&input_object;

	for(
		json::path::const_iterator current_member = 
			p.begin(); 
		current_member != p.end(); 
		++current_member)
	{
		sge::parse::json::member_vector::const_iterator it = 
			std::find_if(
				current_object->members.begin(),
				current_object->members.end(),
				sge::parse::json::member_name_equal(
					*current_member));

		if(it == current_object->members.end())
			throw 
				fruitlib::exception(
					FCPPT_TEXT("Couldn't navigate to \"")+
					json::path_to_string(
						p)+
					FCPPT_TEXT("\", stopped at \"")+
					(*current_member)+
					FCPPT_TEXT("\" because we couldn't find the object here!"));

		if(typeid(it->value.type()) != typeid(sge::parse::json::object))
			throw 
				fruitlib::exception(
					FCPPT_TEXT("Couldn't navigate to \"")+
					json::path_to_string(
						p)+
					FCPPT_TEXT("\", stopped at \"")+
					(*current_member)+
					FCPPT_TEXT("\" because this member has type \"")+
					fcppt::type_name(
						it->value.type())+
					FCPPT_TEXT("\" instead of type sge::parse::json::object!"));

		current_object = 
			&boost::get<sge::parse::json::object>(
				it->value);
	}

	return 
		*current_object;
}

sge::parse::json::object &
fruitcut::fruitlib::json::find_object_exn(
	sge::parse::json::object &input_object,
	json::path const &p)
{
	return 
		const_cast<sge::parse::json::object &>(
			json::find_object_exn(
				input_object,
				p));
}
