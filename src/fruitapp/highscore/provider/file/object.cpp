#include "object.hpp"
#include "connection.hpp"
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>

fruitapp::highscore::provider::file::object::object(
	fcppt::filesystem::path const &_path)
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
		fruitapp::highscore::provider::connection_base_ptr(
			fcppt::make_unique_ptr<file::connection>(
				path_));
}

fruitapp::highscore::provider::file::object::~object()
{
}
