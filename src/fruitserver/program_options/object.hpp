#ifndef FRUITSERVER_PROGRAM_OPTIONS_OBJECT_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_OBJECT_HPP_INCLUDED

#include <fruitserver/program_options/detail/bad_cast.hpp>
#include <fruitserver/program_options/detail/holder_get.hpp>
#include <fruitserver/program_options/option_sequence.hpp>
#include <fruitserver/program_options/detail/option_holder_base_ptr.hpp>
#include <fruitserver/program_options/command_line_parameters.hpp>
#include <tr1/unordered_map>
#include <string>
#include <ostream>
#include <stdexcept>

namespace fruitserver
{
namespace program_options
{
class object
{
public:
	explicit
	object(
		program_options::option_sequence const &,
		program_options::command_line_parameters const &);

	template<typename T>
	T const
	get(
		std::string const &name) const
	{
		name_to_holder::const_iterator const found =
			options_.find(
				name);

		if(found == options_.end())
			throw
				std::runtime_error(
					"Couldn't find option with name \""+
					name+
					"\"");

		try
		{
			return
				detail::holder_get<T>(
					*(found->second));
		}
		catch(
			detail::bad_cast const &e)
		{
			throw
				std::runtime_error(
					"Type error in program option \""+
					name+
					"\": "+
					e.what());
		}
	}

	std::string const
	help() const;

	~object();
private:
	typedef
	std::tr1::unordered_map<std::string,detail::option_holder_base_ptr>
	name_to_holder;

	name_to_holder options_;
};
}
}

#endif
