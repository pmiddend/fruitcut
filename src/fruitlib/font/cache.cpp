#include <fruitlib/font/cache.hpp>
#include <fruitlib/font/drawer/object.hpp>
#include <fruitlib/font/drawer/parameters.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <sge/font/system_ptr.hpp>
#include <sge/font/bitmap/create.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/parse/json/json.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/system.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/filesystem/extension_without_dot.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/tr1/type_traits.hpp>
#include <boost/next_prior.hpp>
#include <iostream>
#include <utility>
#include <map>

fruitlib::font::cache::cache(
	sge::font::system &_font_system,
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader,
	fcppt::filesystem::path const &_base_path,
	sge::parse::json::object const &_fonts)
:
	metrics_(),
	drawers_(),
	to_metrics_(),
	to_drawer_(),
	base_path_(
		_base_path)
{
	typedef
	std::map
	<
		fcppt::string,
		std::pair<metrics_sequence::iterator,drawer_sequence::iterator>
	>
	bitmap_font_cache;

	typedef
	std::map
	<
		std::pair<fcppt::string,sge::font::size_type>,
		std::pair<metrics_sequence::iterator,drawer_sequence::iterator>
	>
	ttf_font_cache;

	bitmap_font_cache bitmap_fonts_;
	ttf_font_cache ttf_fonts_;

	for(
		sge::parse::json::member_vector::const_iterator current_font_raw =
			_fonts.members.begin();
		current_font_raw != _fonts.members.end();
		++current_font_raw)
	{
		fcppt::string const current_identifier = 
			current_font_raw->name;

		sge::parse::json::object const &current_font = 
			sge::parse::json::get<sge::parse::json::object>(
				current_font_raw->value);

		fcppt::string const filename = 
			sge::parse::json::find_member_exn<sge::parse::json::string>(
				current_font.members,
				FCPPT_TEXT("filename"));

		if(fcppt::filesystem::extension_without_dot(filename) == FCPPT_TEXT("ttf"))
		{
			sge::font::size_type font_size = 
				json::find_and_convert_member<sge::font::size_type>(
					current_font,
					json::path(
						FCPPT_TEXT("size")));
		
			// (string,size) -> (metrics_it,drawer_it)
			ttf_font_cache::iterator cached_value = 
				ttf_fonts_.find(
					std::make_pair(
						filename,
						font_size));

			if (cached_value == ttf_fonts_.end())
			{
				metrics_.push_back(
					_font_system.create_font(
						base_path_/FCPPT_TEXT("fonts")/filename,
						font_size));
				fcppt::container::ptr::push_back_unique_ptr(
					drawers_,
					fcppt::make_unique_ptr<fruitlib::font::drawer::object>(
						fruitlib::font::drawer::parameters(
							_renderer)));
				cached_value = 
					ttf_fonts_.insert(
						std::make_pair(
							std::make_pair(
								filename,
								font_size),
							std::make_pair(
								boost::prior(
									metrics_.end()),
								boost::prior(
									drawers_.end())))).first;
			}

			to_metrics_.insert(
				std::make_pair(
					current_identifier,
					cached_value->second.first));
			to_drawer_.insert(
				std::make_pair(
					current_identifier,
					cached_value->second.second));
		}
		else
		{
			// string -> (metrics_it,drawer_it)
			bitmap_font_cache::const_iterator cached_value = 
				bitmap_fonts_.find(
					filename);

			if (cached_value == bitmap_fonts_.end())
			{
				metrics_.push_back(
					sge::font::bitmap::create(
						base_path_/FCPPT_TEXT("fonts")/filename,
						_image_loader));
				fcppt::container::ptr::push_back_unique_ptr(
					drawers_,
					fcppt::make_unique_ptr<fruitlib::font::drawer::object>(
						fruitlib::font::drawer::parameters(
							_renderer)));
				cached_value = 
					bitmap_fonts_.insert(
						std::make_pair(
							filename,
							std::make_pair(
								boost::prior(
									metrics_.end()),
								boost::prior(
									drawers_.end())))).first;
			}

			to_metrics_.insert(
				std::make_pair(
					current_identifier,
					cached_value->second.first));
			to_drawer_.insert(
				std::make_pair(
					current_identifier,
					cached_value->second.second));
		}
	}

	FCPPT_ASSERT(
		to_metrics_.size() == to_drawer_.size());
}

sge::font::metrics_ptr const
fruitlib::font::cache::metrics(
	fcppt::string const &identifier)
{
	to_metrics::const_iterator i = 
		to_metrics_.find(
			identifier);
	FCPPT_ASSERT_MESSAGE(
		i != to_metrics_.end(),
		FCPPT_TEXT("Font called ")+identifier+FCPPT_TEXT(" not found!"));
	FCPPT_ASSERT(
		*(i->second));
	return *(i->second);
}

fruitlib::font::drawer::object &
fruitlib::font::cache::drawer(
	fcppt::string const &identifier)
{
	to_drawer::const_iterator i = 
		to_drawer_.find(
			identifier);
	FCPPT_ASSERT_MESSAGE(
		i != to_drawer_.end(),
		FCPPT_TEXT("Font called ")+identifier+FCPPT_TEXT(" not found!"));
	return *(i->second);
}

fruitlib::font::cache::~cache()
{
}
