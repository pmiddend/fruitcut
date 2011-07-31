#include <fruitserver/highscore_to_plain_file.hpp>
#include <fruitserver/separator.hpp>
#include <fruitserver/lexical_cast.hpp>
#include <fruitserver/byte_sequence.hpp>
#include <fruitserver/ascii/from_native.hpp>
#include <fruitserver/ascii/to_byte_sequence.hpp>
#include <fstream>
#include <stdexcept>

void
fruitserver::highscore_to_plain_file(
	fruitserver::highscore_sequence const &h,
	fruitserver::path const &path)
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
		fruitserver::highscore_sequence::const_iterator it = 
			h.begin();
		it != h.end();
		++it)
	{
		result += it->name();
		result.append(
			static_cast<ascii::string::size_type>(
				1),
			fruitserver::separator());
		result += 
			ascii::from_native(
				fruitserver::lexical_cast<std::string>(
					it->score()));
		result.append(
			static_cast<ascii::string::size_type>(
				1),
			fruitserver::separator());
		result += it->datetime();
		result.append(
			static_cast<ascii::string::size_type>(
				1),
			fruitserver::separator());
	}

	fruitserver::byte_sequence const bytes = 
		ascii::to_byte_sequence(
			result);

	output_stream.write(
		reinterpret_cast<char const *>(
			&result[0]),
		static_cast<std::streamsize>(
			result.size()));
}
