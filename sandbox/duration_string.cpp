//#include "../string_to_duration.hpp"
#include <boost/optional.hpp>
#include <boost/fusion/container/vector/vector10.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/array.hpp>
#include <string>

int main()
{
	typedef
	boost::fusion::vector7
	<
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>
	>
	parse_result_type;

	/*
	typedef
	boost::array<boost::optional<long>,7>
	parse_result_type;
	*/

	std::string const input;
	std::string::const_iterator it = input.begin();

	parse_result_type parse_result;

	boost::spirit::qi::parse(
		it,
		input.end(),
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('d')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('h')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('m') >> !boost::spirit::qi::lit('s')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('s')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('m') >> boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('u') >> boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('n') >> boost::spirit::qi::lit('s')),
		parse_result);
	/*
	fcppt::optional<fcppt::chrono::seconds> const result = 
		fruitcut::string_to_duration<fcppt::chrono::seconds>(
			std::string("1d"));

	if (!result)
		std::cout << "!result\n";
	else
		std::cout 
			<< 
				result->count() 
			<< "\n";
	*/
}

