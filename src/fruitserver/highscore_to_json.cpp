#include "highscore_to_json.hpp"
#include "lexical_cast.hpp"
#include "ascii/from_native.hpp"
#include <string>

fruitserver::ascii::string const
fruitserver::highscore_to_json(
	fruitserver::highscore_sequence const &scores)
{
	ascii::string result;

	result += 
		ascii::from_native("{ \"entries\" : [");

	for(
		fruitserver::highscore_sequence::const_iterator it = 
			scores.begin();
		it != scores.end();
		++it)
	{
		result += ascii::from_native("{ \"name\" : \"")+(it->name())+ascii::from_native("\", ");
		result += ascii::from_native("\"score\" : "+fruitserver::lexical_cast<std::string>(it->score()))+ascii::from_native(", ");
		result += ascii::from_native("\"date-time\" : \"")+it->datetime()+ascii::from_native("\"");
		result += ascii::from_native("}");
		if(it != --scores.end())
			result += ascii::from_native(",");
	}

	result += ascii::from_native(" ] }");

	return 	
		result;
}
