#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <string>

int main()
{
	namespace qi = boost::spirit::qi;

	typedef
	std::string
	string;

	typedef string::iterator iterator;

	string input;
	std::cout << "Input: ";
	std::getline(
		std::cin,
		input);

	iterator start = input.begin(),end = input.end();

	string protocol,url;
	unsigned port = 1337;

	bool res = 
		qi::parse(
			start,
			end,
			(qi::as_string[+~qi::standard::char_(':')])[boost::phoenix::ref(protocol) = qi::_1] >> 
			qi::lit("://") >> 
			(
				((qi::as_string[+~qi::char_(':')])[boost::phoenix::ref(url) = qi::_1] >> qi::lit(':') >> qi::uint_[boost::phoenix::ref(port) = qi::_1]) | 
				(qi::as_string[+qi::standard::char_])[boost::phoenix::ref(url) = qi::_1]));

	if (start == end)
		std::cout << "Reached end of file. protocol: " << protocol << ", url: " << url << ", port: " << port << std::endl;
	if (res)
		std::cout << "success!" << std::endl;
	else
		std::cout << "failure!" << std::endl;
}
