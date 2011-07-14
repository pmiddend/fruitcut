#include "process_command.hpp"
#include "current_datetime.hpp"
#include "ascii_to_native.hpp"
#include "highscore_to_json.hpp"
#include "lexical_cast.hpp"
#include "highscore_from_file.hpp"
#include "ascii_to_native_char.hpp"
#include "highscore_to_string.hpp"
#include "native_to_ascii.hpp"
#include "logger.hpp"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <fstream>
#include <cstddef>

namespace
{
bool
numerical_score_comparator(
	fruitcut::server::highscore_entry const &a,
	fruitcut::server::highscore_entry const &b)
{
	return a.score_numerical() > b.score_numerical();
}
}

namespace
{
std::string
make_error_response(
	std::string const &s)
{
	return "{ \"error\" : \""+s+"\" }";
}
}

std::string
fruitcut::server::process_command(
	std::ostream &log_stream,
	server::command const &c,
	std::string const &data_dir)
{
	if(
		c.empty())
	{
		server::logger(log_stream) 
			<< std::string(": Error: got an empty command");
		return 
			make_error_response("Got an empty command");
	}

	if(c[0].size() != 1u)
	{
		server::logger(log_stream) 
			<< ": Error: got an invalid command: " 
			<< server::ascii_to_native(c[0]);
		return
			make_error_response(
				"Command '"+c[0]+"' invalid");
	}


	char const command_name = 
		server::ascii_to_native_char(
			c[0][0]);

	if(command_name == 'G')
	{
		if(c.size() != 2u)
		{
			server::logger(log_stream) 
				<< ": Error: the command: " 
				<< server::ascii_to_native(c[0])
				<< " doesn't get two parameters";
			return
				make_error_response(
					"Command '"+c[0]+"' got an invalid number of parameters");
		}

		return 
			server::highscore_to_json(
				server::highscore_from_file(
					data_dir+"/"+c[1]));
	}

	if(command_name == 'P')
	{
		if(c.size() != 4u)
		{
			server::logger(log_stream) 
				<< ": Error: the command: " 
				<< server::ascii_to_native(c[0])
				<< " doesn't get four parameters";
			return
				make_error_response(
					"Command '"+c[0]+"' got an invalid number of parameters");
		}

		std::string const 
			revision = 
				c[1],
			score = 
				c[2],
			name = 
				c[3];

		server::highscore_sequence highscore(
			server::highscore_from_file(
				data_dir+"/"+revision));

		server::logger(log_stream) 
			<< ": Got a highscore with " 
			<< highscore.size() 
			<< " entries";

		server::logger(log_stream) 
			<< "Adding highscore entry with name " 
			<< name
			<< " and score " 
			<< score;

		server::highscore_entry new_entry(
			name,
			score);

		highscore.push_back(
			new_entry);

		std::sort(
			highscore.begin(),
			highscore.end(),
			&numerical_score_comparator);

		std::size_t const rank = 
			static_cast<std::size_t>(
				std::distance(
					highscore.begin(),
					std::find(
						highscore.begin(),
						highscore.end(),
						new_entry))+1u);

		std::string const output = 
			server::highscore_to_string(
				highscore);

		std::ofstream output_stream(
			(data_dir+"/"+revision).c_str());

		if(!output_stream.is_open())
			throw 
				std::runtime_error(
					"Couldn't open file \""+(data_dir+"/"+revision)+"\"");

		output_stream.write(
			&output[0],
			static_cast<std::streamsize>(
				output.size()));

		server::logger(log_stream) << ": Write completed, writing rank " << rank << "...";

		return 
			server::ascii_to_native("{\"rank\" : "+server::lexical_cast<std::string>(rank)+"}");
	}

	return
		make_error_response(
			"Command '"+c[0]+"' is not known to this server");
}
