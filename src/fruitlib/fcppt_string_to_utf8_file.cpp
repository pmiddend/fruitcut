#include <fruitlib/fcppt_string_to_utf8_file.hpp>
#include <sge/charconv/fcppt_string_to_utf8.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/fstream.hpp>
#include <fcppt/config/external_end.hpp>


bool
fruitlib::fcppt_string_to_utf8_file(
	fcppt::string const &_string,
	boost::filesystem::path const &_path,
	sge::charconv::system &_system)
{
	boost::filesystem::ofstream file(
		_path);

	if(!file.is_open())
		return false;

	sge::charconv::utf8_string const result(
		sge::charconv::fcppt_string_to_utf8(
			_system,
			_string));

	return
		!file.write(
			reinterpret_cast<char const *>(
				result.c_str()),
			static_cast<std::streamsize>(
				result.size())).fail();
}
