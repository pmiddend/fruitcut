#include "media_path.hpp"
#include <fruitcut/build/media_path.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>


boost::filesystem::path const
fruitcut::media_path()
{
	return fruitcut::build_media_path();
}
