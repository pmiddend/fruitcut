#include <fruitapp/media_path.hpp>
#include <fruitcut/build/media_path.hpp>
#include <fcppt/filesystem/path.hpp>

fcppt::filesystem::path const
fruitapp::media_path()
{
	return fruitcut::build_media_path();
}
