#include <fruitlib/exception.hpp>
#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <fruitlib/utf8_file_to_fcppt_string_exn.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path_to_string.hpp>


fcppt::string const
fruitlib::utf8_file_to_fcppt_string_exn(
	sge::charconv::system &charconv_system,
	boost::filesystem::path const &path)
{
	fcppt::optional<fcppt::string> const optional_content =
		fruitlib::utf8_file_to_fcppt_string(
			charconv_system,
			path);

	if(!optional_content)
		throw
			fruitlib::exception(
				FCPPT_TEXT("Couldn't open file \"")+
				fcppt::filesystem::path_to_string(
					path)+
				FCPPT_TEXT("\n"));

	return *optional_content;
}
