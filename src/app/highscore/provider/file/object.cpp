#include "object.hpp"
#include "connection.hpp"
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>

fruitcut::app::highscore::provider::file::object::object(
	fcppt::filesystem::path const &_path)
:
	path_(
		_path)
{
}

fruitcut::app::highscore::provider::identifier::value_type const
fruitcut::app::highscore::provider::file::object::identifier() const
{
	return 
		FCPPT_TEXT("File \"")+
		fcppt::filesystem::stem(
			path_)+
		FCPPT_TEXT("\"");
}

fruitcut::app::highscore::provider::connection_base_ptr
fruitcut::app::highscore::provider::file::object::create_connection()
{
	return 
		fruitcut::app::highscore::provider::connection_base_ptr(
			fcppt::make_unique_ptr<file::connection>(
				path_));
}

fruitcut::app::highscore::provider::file::object::~object()
{
}
