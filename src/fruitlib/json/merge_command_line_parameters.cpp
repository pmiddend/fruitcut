#include "merge_command_line_parameters.hpp"
#include "string_to_value.hpp"
#include "../command_line_parameters.hpp"
#include "../exception.hpp"
#include <sge/parse/json/json.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/algorithm/shortest_levenshtein.hpp>
#include <fcppt/algorithm/map.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/range/numeric.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ostream>

namespace
{
void
process_option(
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
		throw fruitcut::fruitlib::exception(
			FCPPT_TEXT("The command line parameter \"")+
			input+
			FCPPT_TEXT("\" has an invalid format. See --help to see what that means."));

	FCPPT_ASSERT(
		!boost::fusion::at_c<0>(result).empty());

	fcppt::string const element =
		boost::fusion::at_c<0>(result).back();

	boost::fusion::at_c<0>(result).pop_back();

	sge::parse::json::object *target = 
		boost::accumulate(
			boost::fusion::at_c<0>(
				result),
			&o,
			&boost::phoenix::bind(
				&sge::parse::json::find_member_exn
				<
					sge::parse::json::object,
					sge::parse::json::member_vector
				>,
				boost::phoenix::bind(
					&sge::parse::json::object::members,
					boost::phoenix::arg_names::arg1),
				boost::phoenix::arg_names::arg2));

	sge::parse::json::member_vector::iterator it = 
		std::find_if(
			target->members.begin(),
			target->members.end(),
			sge::parse::json::member_name_equal(
				element));

	if (it == target->members.end())
		throw fruitcut::fruitlib::exception(
			FCPPT_TEXT("Couldn't find member \"")+
			element+
			FCPPT_TEXT("\", did you mean: ")+
			fcppt::algorithm::shortest_levenshtein(
				fcppt::algorithm::map<std::vector<fcppt::string> >(
					target->members,
					boost::phoenix::bind(
						&sge::parse::json::member::name,
						boost::phoenix::arg_names::arg1)),
				element));

	it->value = 
		fruitcut::fruitlib::json::string_to_value(
			boost::fusion::at_c<1>(
				result));
}
}

sge::parse::json::object const
fruitcut::fruitlib::json::merge_command_line_parameters(
	sge::parse::json::object input,
	fruitlib::command_line_parameters const &parameters)
{
	if (parameters.size() >= 2 && parameters[1] == FCPPT_TEXT("--help"))
	{
		std::cout << 
			"Command line options are of the form:\n\n"
			"foo/bar/baz=qux\n\n"
			"where foo/bar/baz is a sequence of objects in the config.json file.\n"
			"qux can be any json type (strings, arrays, etc.)\n"
			"For example, if config.json looks like this:\n\n"
			"{ \"renderer\" : { \"screen_size\" : [640,480] } }\n\n"
			"You could change the resolution via:\n\n"
			"renderer/screen_size=[1024,768]\n\n"
			"Be aware of two things, though:\n"
			"1. You can use _any_ json type, so it's possible to write\n\n"
			"renderer/screen_size=1.0\n\n"
			"which, of course, makes no sense. Try to use the correct type.\n"
			"2. Be aware of your shell' special characters. For example, in bash\n"
			"to set a json string, you have to write:\n\n"
			"player/name='\"foobar\"'\n\n"
			"It's a good idea to always put the argument in apostrophes.\n";
		throw 
			fruitlib::exception(
				FCPPT_TEXT("Help was needed"));
	}

	for(
		fruitlib::command_line_parameters::const_iterator i = 
			++parameters.begin(); 
		i != parameters.end(); 
		++i)
		::process_option(
			input,
			*i);

	return 
		input;
}
