#include "highscore_to_json.hpp"
#include "native_to_ascii.hpp"
#include "lexical_cast.hpp"
#include <string>

std::string const
fruitcut::server::highscore_to_json(
	server::highscore_sequence const &scores)
{
	std::string result;

	result += 
		server::native_to_ascii("{ \"entries\" : [");

	for(
		server::highscore_sequence::const_iterator it = 
			scores.begin();
		it != scores.end();
		++it)
	{
		result += server::native_to_ascii("{ \"name\" : \"")+(it->name())+server::native_to_ascii("\", ");
		result += server::native_to_ascii("\"score\" : "+server::lexical_cast<std::string>(it->score()))+server::native_to_ascii(", ");
		result += server::native_to_ascii("\"date-time\" : \"")+it->datetime()+server::native_to_ascii("\"");
		result += server::native_to_ascii("}");
		if(it != --scores.end())
			result += server::native_to_ascii(",");
	}

	result += server::native_to_ascii(" ] }");

	return 	
		result;
}
