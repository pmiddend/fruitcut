#include "highscore_to_json.hpp"
#include "lexical_cast.hpp"
#include "ascii/from_native.hpp"
#include <string>

fruitcut::server::ascii::string const
fruitcut::server::highscore_to_json(
	server::highscore_sequence const &scores)
{
	ascii::string result;

	result += 
		ascii::from_native("{ \"entries\" : [");

	for(
		server::highscore_sequence::const_iterator it = 
			scores.begin();
		it != scores.end();
		++it)
	{
		result += ascii::from_native("{ \"name\" : \"")+(it->name())+ascii::from_native("\", ");
		result += ascii::from_native("\"score\" : "+server::lexical_cast<std::string>(it->score()))+ascii::from_native(", ");
		result += ascii::from_native("\"date-time\" : \"")+it->datetime()+ascii::from_native("\"");
		result += ascii::from_native("}");
		if(it != --scores.end())
			result += ascii::from_native(",");
	}

	result += ascii::from_native(" ] }");

	return 	
		result;
}
