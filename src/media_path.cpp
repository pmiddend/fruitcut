#include "media_path.hpp"
#include <fruitcut/build/media_path.hpp>

fcppt::filesystem::path const
fruitcut::media_path()
{
	return fruitcut::build_media_path();
}
