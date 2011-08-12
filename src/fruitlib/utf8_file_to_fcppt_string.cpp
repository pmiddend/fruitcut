#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <fruitlib/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/io/cifstream.hpp>
#include <fcppt/string.hpp>
#include <fcppt/utf8/from_std_string.hpp>
#include <fcppt/utf8/to_fcppt_string.hpp>
#include <iterator>
#include <string>

fcppt::string const
fruitlib::utf8_file_to_fcppt_string(
	fcppt::filesystem::path const &path)
{
	fcppt::io::cifstream file_stream(
		path);

	if(!file_stream.is_open())
		throw
			fruitlib::exception(
				FCPPT_TEXT("Couldn't open file \"")+
				fcppt::filesystem::path_to_string(
					path)+
				FCPPT_TEXT("\n"));

	return
		fcppt::utf8::to_fcppt_string(
			fcppt::utf8::from_std_string(
				std::string(
					std::istreambuf_iterator<char>(
						file_stream),
					std::istreambuf_iterator<char>())));
}
