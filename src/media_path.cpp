#include "media_path.hpp"
#include <fcppt/filesystem/path.hpp>
#include <fcppt/config/external_begin.hpp>
#include <fruitcut/build/media_path.hpp>
#include <fcppt/config/external_end.hpp>


fcppt::filesystem::path const
fruitcut::media_path()
{
	return fruitcut::build_media_path();
}
