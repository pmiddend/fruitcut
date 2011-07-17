#include "highscore_to_plain_file.hpp"
#include "separator.hpp"
#include "lexical_cast.hpp"
#include "byte_sequence.hpp"
#include "ascii/from_native.hpp"
#include "ascii/to_byte_sequence.hpp"
#include <fstream>
#include <stdexcept>

void
fruitcut::server::highscore_to_plain_file(
	server::highscore_sequence const &h,
	server::path const &path)
{
	std::ofstream output_stream(
		path.c_str());

	if(!output_stream.is_open())
		throw 
			std::runtime_error(
				"Couldn't open file \""+
				path+
				"\"");

	ascii::string result;

	for(
		server::highscore_sequence::const_iterator it = 
			h.begin();
		it != h.end();
		++it)
	{
		result += it->name();
		result.append(
			static_cast<ascii::string::size_type>(
				1),
			server::separator());
		result += 
			ascii::from_native(
				server::lexical_cast<std::string>(
					it->score()));
		result.append(
			static_cast<ascii::string::size_type>(
				1),
			server::separator());
		result += it->datetime();
		result.append(
			static_cast<ascii::string::size_type>(
				1),
			server::separator());
	}

	server::byte_sequence const bytes = 
		ascii::to_byte_sequence(
			result);

	output_stream.write(
		reinterpret_cast<char const *>(
			&result[0]),
		static_cast<std::streamsize>(
			result.size()));
}
