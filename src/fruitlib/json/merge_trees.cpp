#include <fruitlib/json/merge_trees.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/member.hpp>
#include <sge/parse/json/member_vector.hpp>
#include <sge/parse/json/member_name_equal.hpp>
#include <fcppt/string.hpp>
#include <fcppt/algorithm/map.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <set>
#include <algorithm>

namespace
{
struct visitor
{
public:
	typedef
	sge::parse::json::value
	result_type;

	result_type
	operator()(
		sge::parse::json::object const &original,
		sge::parse::json::object const &update) const
	{
		return 
			fruitlib::json::merge_trees(
				original,
				update);
	}

	template<typename T1,typename T2>
	result_type
	operator()(
		T1 const &,
		T2 const &update) const
	{
		return update;
	}
};
}

namespace fruitlib
{
namespace stdlib
{
template<typename T>
std::set<T> const
union_(
	std::set<T> result,
	std::set<T> const &t)
{
	std::copy(
		t.begin(),
		t.end(),
		std::inserter(
			result,
			result.end()));
	return result;
}
}
}

namespace fruitlib
{
namespace json
{
std::set<fcppt::string> const
key_set(
	sge::parse::json::object const &o)
{
	return 
		fcppt::algorithm::map<std::set<fcppt::string> >(
			o.members,
			boost::phoenix::bind(
				&sge::parse::json::member::name,
				boost::phoenix::arg_names::arg1));
}
}
}

sge::parse::json::object const
fruitlib::json::merge_trees(
	sge::parse::json::object const &original,
	sge::parse::json::object const &update)
{
	using namespace sge::parse::json;

	sge::parse::json::object result;

	typedef
	std::set<fcppt::string>
	string_set;

	string_set const union_set = 
		fruitlib::stdlib::union_(
			key_set(
				original),
			key_set(
				update));

	for(
		string_set::const_iterator key = 
			union_set.begin();
		key != union_set.end();
		++key)
	{
		member_vector::const_iterator 
			original_it = 
				std::find_if(
					original.members.begin(),
					original.members.end(),
					member_name_equal(
						*key)),
			update_it = 
				std::find_if(
					update.members.begin(),
					update.members.end(),
					member_name_equal(
						*key));

		// Object exists only in the update? Then copy
		if (original_it == original.members.end())
		{
			result.members.push_back(
				*update_it);
			continue;
		}
		
		// Object exists only in the original? Then copy
		if (update_it == update.members.end())
		{
			result.members.push_back(
				*original_it);
			continue;
		}

		// Both objects have the key, then merge!
		result.members.push_back(
			member(
				*key,
				boost::apply_visitor(
					visitor(),
					original_it->value,
					update_it->value)));
	}

	return result;
}
