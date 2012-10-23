#include <fruitapp/media_path.hpp>
#include <fruitapp/build/fruitapp_media_path.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>


boost::filesystem::path const
fruitapp::media_path()
{
	return fruitapp::build_fruitapp_media_path();
}
