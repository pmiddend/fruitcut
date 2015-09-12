#include <fruitapp/highscore/provider/file/connection.hpp>
#include <fruitapp/highscore/provider/file/object.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr_to_base.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>


fruitapp::highscore::provider::file::object::object(
	boost::filesystem::path const &_path)
:
	path_(
		_path)
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
		fcppt::unique_ptr_to_base<
			fruitapp::highscore::provider::connection_base
		>(
			fcppt::make_unique_ptr<file::connection>(
				path_));
}

fruitapp::highscore::provider::file::object::~object()
{
}
