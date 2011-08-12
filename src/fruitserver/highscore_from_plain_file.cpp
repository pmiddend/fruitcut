#include <fruitserver/highscore_from_plain_file.hpp>
#include <fruitserver/separator.hpp>
#include <fruitserver/ascii/string.hpp>
#include <fruitserver/ascii/char_traits.hpp>
#include <fruitserver/ascii/to_native.hpp>
#include <fruitserver/lexical_cast.hpp>
#include <string>
#include <stdexcept>
#include <fstream>

namespace
{
std::istream &
mygetline(
	std::istream &in,
	fruitserver::ascii::string &str,
	fruitserver::ascii::char_ const &delim)
{
	typedef std::istream::ios_base ios_base;
	typedef std::istream::int_type int_type;
	typedef fruitserver::ascii::string::size_type size_type;
	typedef fruitserver::ascii::char_traits char_traits;

	size_type extracted = 0;
	size_type const n =
		str.max_size();
	ios_base::iostate err =
		ios_base::goodbit;
	std::istream::sentry
		cerb(
			in,
			true);

	if (cerb)
	{
		str.erase();
		int_type const idelim =
			char_traits::to_int_type(
				delim);
		int_type const eof =
			char_traits::eof();

		int_type c =
			in.rdbuf()->sgetc();

		while(
			extracted < n &&
			!char_traits::char_traits::eq_int_type(c, eof) &&
			!char_traits::eq_int_type(c, idelim))
		{
			str +=
				char_traits::to_char_type(
					c);
			++extracted;
			c =
				in.rdbuf()->snextc();
		}

		if(
			char_traits::eq_int_type(
				c,
				eof))
			err |=
				ios_base::eofbit;
		else if(
			char_traits::eq_int_type(
				c,
				idelim))
		{
			++extracted;
			in.rdbuf()->sbumpc();
		}
		else
		{
			err |= ios_base::failbit;
		}
	}

	if (!extracted)
		err |= ios_base::failbit;
	if (err)
		in.setstate(
			err);
	return in;
}
}

fruitserver::highscore_sequence const
fruitserver::highscore_from_plain_file(
	fruitserver::path const &s)
{
	std::ifstream stream(
		s.c_str());

	fruitserver::highscore_sequence result;

	if(!stream.is_open())
		return result;

	ascii::string name,score,datetime;
	while(
		mygetline(
			stream,
			name,
			fruitserver::separator()) &&
		mygetline(
			stream,
			score,
			fruitserver::separator()) &&
		mygetline(
			stream,
			datetime,
			fruitserver::separator()))
	{
		result.push_back(
			fruitserver::highscore_entry(
				name,
				fruitserver::lexical_cast<fruitserver::score>(
					ascii::to_native(
						score)),
				datetime));
	}

	return result;
}
