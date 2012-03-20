#include <fruitserver/program_options/command_line_parameters.hpp>
#include <fruitserver/program_options/help_was_needed.hpp>
#include <fruitserver/program_options/object.hpp>
#include <fruitserver/program_options/option_sequence.hpp>
#include <stdexcept>
#include <string>


fruitserver::program_options::object::object(
	program_options::option_sequence const &_options,
	program_options::command_line_parameters const &_command_line)
:
	options_()
{
	for(
		program_options::option_sequence::const_iterator it =
			_options.begin();
		it != _options.end();
		++it)
		options_[it->name()] =
			it->holder();

	for(
		program_options::command_line_parameters::const_iterator it =
			_command_line.begin();
		it != _command_line.end();
		++it)
	{
		std::string current_argument =
			*it;

		if(current_argument.substr(0,2) != "--")
			throw
				std::runtime_error(
					"Error parsing command line argument \""+
					(*it)+
					"\": Arguments have to begin with \"--\"");

		current_argument.erase(
			current_argument.begin(),
			current_argument.begin()+2);

		std::string::size_type const position_of_equal =
			current_argument.find('=');

		if(position_of_equal == std::string::npos)
		{
			// A little special case
			if(current_argument == "help")
				throw
					help_was_needed(
						help());

			throw
				std::runtime_error(
					"Error parsing command line argument \""+
					current_argument+
					"\": Arguments have to be of the form \"foo=bar\"");
		}

		std::string const
			name =
				current_argument.substr(
					0,
					position_of_equal),
			value =
				current_argument.substr(
					position_of_equal+1u);

		name_to_holder::const_iterator const found =
			options_.find(
				name);

		if(found == options_.end())
			throw
				std::runtime_error(
					"Couldn't find option with name \""+
					name+
					"\"");

		found->second->from_string(
			value);
	}
}

std::string const
fruitserver::program_options::object::help() const
{
	std::string result;
	result += "Available options and their values:\n\n";
	for(name_to_holder::const_iterator it = options_.begin(); it != options_.end(); ++it)
		result += "--"+(it->first)+"=\""+(it->second->to_string())+"\"\n";
	return result;
}

fruitserver::program_options::object::~object()
{
}
