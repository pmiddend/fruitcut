#include <fruitlib/exception.hpp>
#include <fruitlib/utf8_file_to_fcppt_string.hpp>
#include <sge/charconv/utf8_string.hpp>
#include <sge/charconv/utf8_string_to_fcppt.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/path.hpp>
#include <iterator>
#include <string>
#include <fcppt/config/external_end.hpp>


fcppt::optional<fcppt::string> const
fruitlib::utf8_file_to_fcppt_string(
	boost::filesystem::path const &path)
{
	boost::filesystem::ifstream file_stream(
		path);

	if(!file_stream.is_open())
		return
			fcppt::optional<fcppt::string>();

	return
		fcppt::optional<fcppt::string>(
			sge::charconv::utf8_string_to_fcppt(
				sge::charconv::utf8_string(
					std::istreambuf_iterator<char>(
						file_stream),
					std::istreambuf_iterator<char>())));
}
