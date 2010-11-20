#include "process_option.hpp"
#include "string_to_value.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/exception.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/algorithm/map.hpp>
#include <fcppt/algorithm/std/accumulate.hpp>
#include <fcppt/algorithm/shortest_levenshtein.hpp>
#include <fcppt/io/cout.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <vector>

void
fruitcut::json::process_option(
	sge::parse::json::object &o,
	fcppt::string const &input)
{
	typedef std::vector<fcppt::string> string_vector;
	typedef boost::fusion::vector<string_vector,fcppt::string> result_type;

	namespace encoding = boost::spirit::standard;

	result_type result;

	if(
		!boost::spirit::qi::parse(
			input.begin(),
			input.end(),
			(
				*~encoding::char_("/=")
				% boost::spirit::lit('/')
			)
			>> boost::spirit::lit('=')
			>> *encoding::char_,
			result ) )
		throw sge::exception(
			FCPPT_TEXT("Invalid format \"")+
			input+
			FCPPT_TEXT("\""));

	//fcppt::io::cout << "Parts were:\n";
	//BOOST_FOREACH(string_vector::const_reference r,boost::fusion::at_c<0>(result))
	//	fcppt::io::cout << r << "\n";

	FCPPT_ASSERT(
		!boost::fusion::at_c<0>(result).empty());

	fcppt::string const element =
		boost::fusion::at_c<0>(result).back();

	boost::fusion::at_c<0>(result).pop_back();

	sge::parse::json::object *target = 
		fcppt::algorithm::std::accumulate(
			boost::fusion::at_c<0>(
				result),
			&o,
			[](
				sge::parse::json::object *o,
				fcppt::string const &s)
			{
				return 
					&sge::parse::json::find_member_exn<sge::parse::json::object>(
						o->members,
						s);
			});

	sge::parse::json::member_vector::iterator it = 
		std::find_if(
			target->members.begin(),
			target->members.end(),
			sge::parse::json::member_name_equal(
				element));

	if (it == target->members.end())
		throw sge::exception(
			FCPPT_TEXT("Couldn't find member \"")+
			element+
			FCPPT_TEXT("\", did you mean: ")+
			fcppt::algorithm::shortest_levenshtein(
				fcppt::algorithm::map<std::vector<fcppt::string>>(
					target->members,
					[](sge::parse::json::member const &m) 
					{ 
						return m.name; 
					}),
				element));

	it->value = 
			string_to_value(
				boost::fusion::at_c<1>(
					result));
}
