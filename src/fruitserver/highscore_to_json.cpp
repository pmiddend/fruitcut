#include <fcppt/config/external_begin.hpp>
#include <fruitserver/highscore_to_json.hpp>
#include <fruitserver/lexical_cast.hpp>
#include <fruitserver/ascii/from_native.hpp>
#include <string>
#include <fcppt/config/external_end.hpp>


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
