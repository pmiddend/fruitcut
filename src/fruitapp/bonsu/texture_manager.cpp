#include <fruitapp/bonsu/texture_manager.hpp>
#include <fruitapp/bonsu/texture_identifier.hpp>
#include <fruitapp/exception.hpp>
#include <fruitapp/media_path.hpp>
#include <sge/image/color/format.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/fragmented_unique_ptr.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/renderer/dim2.hpp>
#include <fcppt/filesystem/directory_iterator.hpp>
#include <fcppt/filesystem/stem.hpp>
#include <fcppt/filesystem/is_directory.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <utility>

fruitapp::bonsu::texture_manager::texture_manager(
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader)
:
	texture_manager_(
		boost::phoenix::construct<sge::texture::fragmented_unique_ptr>(
			boost::phoenix::new_<sge::texture::rect_fragmented>(
				fcppt::ref(
					_renderer),
				sge::image::color::format::rgba8,
				sge::renderer::texture::mipmap::off(),
				sge::renderer::dim2(
					512,
					512)))),
	textures_()
{
	for(
		fcppt::filesystem::directory_iterator current_file(
			fruitapp::media_path()/FCPPT_TEXT("bonsu"));
		current_file != fcppt::filesystem::directory_iterator();
		++current_file)
	{
		if(fcppt::filesystem::is_directory(*current_file))
			throw
				fruitapp::exception(
					FCPPT_TEXT("The bonsu directory is supposed to contain only files!"));

		textures_.insert(
			std::make_pair(
				fcppt::filesystem::stem(
					*current_file),
				sge::texture::add_image(
					texture_manager_,
					*_image_loader.load(
						*current_file))));
	}
}

sge::texture::part_ptr const
fruitapp::bonsu::texture_manager::lookup(
	bonsu::texture_identifier const &name_)
{
	texture_map::const_iterator it =
		textures_.find(
			name_);

	if(it == textures_.end())
			throw
				fruitapp::exception(
					FCPPT_TEXT("The requested texture \"")+name_+FCPPT_TEXT("\" was not found!"));

	return
		it->second;
}

fruitapp::bonsu::texture_manager::~texture_manager()
{
}
