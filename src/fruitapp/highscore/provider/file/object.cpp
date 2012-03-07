#include <fruitapp/highscore/provider/file/connection.hpp>
#include <fruitapp/highscore/provider/file/object.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/filesystem/stem.hpp>


fruitapp::highscore::provider::file::object::object(
	sge::charconv::system &_charconv_system,
	fcppt::filesystem::path const &_path)
:
	path_(
		_path),
	charconv_system_(
		_charconv_system)
{
}

fruitapp::highscore::provider::identifier::value_type const
fruitapp::highscore::provider::file::object::identifier() const
{
	return
		FCPPT_TEXT("File \"")+
		fcppt::filesystem::stem(
			path_)+
		FCPPT_TEXT("\"");
}

fruitapp::highscore::provider::connection_base_ptr
fruitapp::highscore::provider::file::object::create_connection()
{
	return
		fruitapp::highscore::provider::connection_base_ptr(
			fcppt::make_unique_ptr<file::connection>(
				fcppt::ref(
					charconv_system_),
				path_));
}

fruitapp::highscore::provider::file::object::~object()
{
}
