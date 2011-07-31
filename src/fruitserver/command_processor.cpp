#include "command_processor.hpp"
#include "logger.hpp"
#include "separator.hpp"
#include "highscore_from_plain_file.hpp"
#include "generate_datetime.hpp"
#include "highscore_to_json.hpp"
#include "highscore_to_plain_file.hpp"
#include "lexical_cast.hpp"
#include "ascii/from_native.hpp"
#include "ascii/from_native_char.hpp"
#include "ascii/to_native.hpp"
#include "ascii/is_letter.hpp"
#include "ascii/is_digit.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>

namespace
{
fruitserver::ascii::string
make_error_response(
	fruitserver::ascii::string const &s)
{
	namespace ascii = fruitserver::ascii;
	if(s.find(ascii::from_native_char('"')) != ascii::string::npos)
		throw 
			std::runtime_error(
				"Error response contains '\"'");
	return 
		ascii::from_native("{ \"error\" : \"")+
		s+
		ascii::from_native("\" }");
}

bool
validate_filename(
	fruitserver::ascii::string const &s)
{
	namespace ascii = fruitserver::ascii;
	for(
		ascii::string::const_iterator it = 
			s.begin();
		it != s.end();
		++it)
		if(!ascii::is_letter(*it) && !ascii::is_digit(*it))
			return false;
	return true;
}

bool
validate_score(
	fruitserver::ascii::string const &s)
{
	namespace ascii = fruitserver::ascii;
	for(
		ascii::string::const_iterator it = 
			s.begin();
		it != s.end();
		++it)
		if(!ascii::is_digit(*it))
			return false;
	return true;
}

bool
validate_name(
	fruitserver::ascii::string const &s)
{
	namespace ascii = fruitserver::ascii;
	for(
		ascii::string::const_iterator it = 
			s.begin();
		it != s.end();
		++it)
		if(it->value() == fruitserver::separator().value())
			return false;
	return true;
}

bool
score_comparator(
	fruitserver::highscore_entry const &a,
	fruitserver::highscore_entry const &b)
{
	return a.score() > b.score();
}
}

fruitserver::command_processor::command_processor(
	std::ostream &_log_stream,
	std::string const &_data_dir)
:	
	log_stream_(
		_log_stream),
	data_dir_(
		_data_dir),
	command_name_to_handler_()
{
	command_name_to_handler_[ascii::from_native("G")] = 
		&command_processor::handle_get;
	command_name_to_handler_[ascii::from_native("P")] = 
		&command_processor::handle_put;
}

fruitserver::ascii::string const
fruitserver::command_processor::process(
	fruitserver::command const &c)
{
	if(c.empty())
	{
		fruitserver::logger(log_stream_) 
			<< ": Error: got an empty command";

		return 
			::make_error_response(
				ascii::from_native(
					"Got an empty command"));
	}

	command_name_to_handler::const_iterator handler = 
		command_name_to_handler_.find(
			c[0]);

	if(handler == command_name_to_handler_.end())
	{
		fruitserver::logger(log_stream_) 
			<< ": Error: Command unknown";

		return 
			::make_error_response(
				ascii::from_native(
					"Unknown command"));
	}

	return 
		(this->*(handler->second))(
			c);
}

fruitserver::command_processor::~command_processor()
{
}

fruitserver::ascii::string const
fruitserver::command_processor::handle_get(
	fruitserver::command const &c)
{
	if(c.size() != 2u)
	{
		fruitserver::logger(log_stream_) 
			<< ": Error: the get command doesn't get two parameters";
		return
			::make_error_response(
				ascii::from_native(
					"Get command got an invalid number of parameters"));
	}

	if(
		!validate_filename(
			c[1]))
	{
		return
			::make_error_response(
				ascii::from_native(
					"Invalid revision id (must only contain ascii letters and digits)"));
	}

	return 
		fruitserver::highscore_to_json(
			fruitserver::highscore_from_plain_file(
				data_dir_+
				"/"+
				ascii::to_native(
					c[1])));
}

fruitserver::ascii::string const
fruitserver::command_processor::handle_put(
	fruitserver::command const &c)
{
	if(c.size() != 4u)
	{
		fruitserver::logger(log_stream_) 
			<< ": Error: the put command gets 3 parameters!";
		return
			make_error_response(
				ascii::from_native(
					"Got an invalid number of parameters"));
	}

	fruitserver::ascii::string const 
		revision = 
			c[1],
		score = 
			c[3],
		name = 
			c[2];

	if(
		!validate_filename(
			revision))
	{
		return
			::make_error_response(
				ascii::from_native(
					"Invalid revision id (must only contain ascii letters and digits)"));
	}

	if(
		!validate_score(
			score))
	{
		return
			::make_error_response(
				ascii::from_native(
					"Invalid score (contains characters other than digits)"));
	}

	if(
		!validate_name(
			name))
	{
		return
			::make_error_response(
				ascii::from_native(
					"Invalid name (contains double quotes)"));
	}

	fruitserver::highscore_sequence highscore(
		fruitserver::highscore_from_plain_file(
			data_dir_+
			"/"+
			ascii::to_native(
				revision)));

	fruitserver::logger(log_stream_) 
		<< ": Got a highscore with " 
		<< highscore.size() 
		<< " entries";

	fruitserver::logger(log_stream_) 
		<< "Adding highscore entry with name " 
		<< ascii::to_native(name)
		<< " and score " 
		<< ascii::to_native(score);

	fruitserver::highscore_entry new_entry(
		name,
		fruitserver::lexical_cast<fruitserver::score>(
			ascii::to_native(
				score)),
		ascii::from_native(
			fruitserver::generate_datetime()));

	highscore.push_back(
		new_entry);

	std::sort(
		highscore.begin(),
		highscore.end(),
		&score_comparator);

	std::size_t const rank = 
		static_cast<std::size_t>(
			std::distance(
				highscore.begin(),
				std::find(
					highscore.begin(),
					highscore.end(),
					new_entry))+1u);

	fruitserver::highscore_to_plain_file(
		highscore,
		data_dir_+
		"/"+
		ascii::to_native(
			revision));

	fruitserver::logger(log_stream_) << ": Write completed, writing rank " << rank << "...";

	return 
		ascii::from_native("{\"rank\" : ")+
		ascii::from_native(
			fruitserver::lexical_cast<std::string>(
				rank))+
		ascii::from_native("}");
}
