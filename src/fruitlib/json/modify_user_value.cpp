#include "modify_user_value.hpp"
#include "find_and_convert_member.hpp"
#include "make_recursive_objects.hpp"
#include "path_to_string.hpp"
#include <fruitlib/exception.hpp>
#include <sge/parse/json/json.hpp>
#include <boost/variant/get.hpp>
#include <fcppt/type_name.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assert.hpp>
#include <numeric>
#include <algorithm>
#include <typeinfo>

void
fruitlib::json::modify_user_value(
	sge::parse::json::object const &structure_json,
	sge::parse::json::object &user_json,
	json::path const &input_path,
	sge::parse::json::value const &new_value)
{
	FCPPT_ASSERT(
		!input_path.empty());

	sge::parse::json::value const old_value = 
		json::find_and_convert_member<sge::parse::json::value>(
			structure_json,
			input_path);

	if(old_value.which() != new_value.which())
		throw 
			fruitlib::exception(
				FCPPT_TEXT("Error trying to update the user configuration node \"")+
				json::path_to_string(
					input_path)+
				FCPPT_TEXT("\", the types of the old and new value didn't match. The old type was \"")+
				fcppt::type_name(old_value.type())+
				FCPPT_TEXT("\", the new type was \"")+
				fcppt::type_name(new_value.type()));

	sge::parse::json::object &target = 
		// 0 is not permitted, 1 would mean: just take a value from
		// user_json, > 1 means: recursively make a path in the user_json
		(std::distance(
			input_path.begin(),
			input_path.end()) > 1)
		?
			json::make_recursive_objects(
				user_json,
				json::path(
					input_path.begin(),
					--input_path.end()))
		:
			user_json;

	sge::parse::json::member_vector::iterator it = 
		boost::find_if(
			target.members,
			sge::parse::json::member_name_equal(
				input_path.back()));

	if(it == target.members.end())
		target.members.push_back(
			sge::parse::json::member(
				input_path.back(),
				new_value));
	else
		it->value = new_value;
}
