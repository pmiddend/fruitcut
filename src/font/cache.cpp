#include "cache.hpp"
#include "drawer/object.hpp"
#include "drawer/parameters.hpp"
#include "../json/find_member.hpp"
#include "../media_path.hpp"
#include <sge/font/system_ptr.hpp>
#include <sge/font/bitmap/create.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/member_vector.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/font/size_type.hpp>
#include <sge/font/system.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/filesystem/extension_without_dot.hpp>
#include <fcppt/tr1/type_traits.hpp>
#include <boost/foreach.hpp>
#include <boost/next_prior.hpp>
#include <boost/static_assert.hpp>
#include <iostream>
#include <utility>
#include <map>

fruitcut::font::cache::cache(
	sge::font::system &_font_system,
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader,
	sge::parse::json::object const &_fonts)
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

	BOOST_FOREACH(
		sge::parse::json::member_vector::const_reference current_font_raw,
		_fonts.members)
	{
		BOOST_STATIC_ASSERT(
			(std::tr1::is_same<sge::parse::json::string,fcppt::string>::value));

		fcppt::string const current_identifier = 
			current_font_raw.name;

		sge::parse::json::object const &current_font = 
			sge::parse::json::get<sge::parse::json::object>(
				current_font_raw.value);

		fcppt::string const filename = 
			sge::parse::json::find_member_exn<sge::parse::json::string>(
				current_font.members,
				FCPPT_TEXT("filename"));

		if(fcppt::filesystem::extension_without_dot(filename) == FCPPT_TEXT("ttf"))
		{
			sge::font::size_type font_size = 
				json::find_member<sge::font::size_type>(
					current_font,
					FCPPT_TEXT("size"));
		
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
						media_path()/FCPPT_TEXT("fonts")/filename,
						font_size));
				drawers_.push_back(
					new fruitcut::font::drawer::object(
						fruitcut::font::drawer::parameters(
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
						media_path()/FCPPT_TEXT("fonts")/filename,
						_image_loader));
				drawers_.push_back(
					new fruitcut::font::drawer::object(
						fruitcut::font::drawer::parameters(
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
fruitcut::font::cache::metrics(
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

fruitcut::font::drawer::object &
fruitcut::font::cache::drawer(
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

fruitcut::font::cache::~cache()
{
}
