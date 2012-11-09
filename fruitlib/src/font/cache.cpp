#include <fruitlib/font/cache.hpp>
#include <sge/font/object.hpp>
#include <sge/font/parameters.hpp>
#include <sge/font/system.hpp>
#include <sge/font/bitmap/create.hpp>
#include <sge/image2d/system.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/member_map.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/parse/json/string.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/pre_message.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/tr1/type_traits.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>
#include <map>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitlib::font::cache::cache(
	sge::font::system &_font_system,
	sge::image2d::system &_image_loader,
	sge::parse::json::object const &_fonts,
	fruitlib::font::base_path const &_base_path)
:
	objects_(),
	to_object_()
{
	typedef
	std::map<fcppt::string,object_sequence::iterator>
	bitmap_font_name_to_object;

	bitmap_font_name_to_object bitmap_font_name_to_object_;

	for(
		sge::parse::json::member_map::const_iterator current_font_raw =
			_fonts.members.begin();
		current_font_raw != _fonts.members.end();
		++current_font_raw)
	{
		fruitlib::font::identifier const current_identifier(
			current_font_raw->first);

		sge::parse::json::object const &current_font =
			sge::parse::json::get<sge::parse::json::object>(
				current_font_raw->second);

		fcppt::string const name =
			sge::parse::json::find_member_exn<sge::parse::json::string>(
				current_font.members,
				FCPPT_TEXT("name"));

		if(name.substr(0,4) == FCPPT_TEXT("ttf:"))
		{
			fcppt::string const real_name(
				name.substr(
					4));

			sge::font::ttf_size const font_size =
				sge::parse::json::find_and_convert_member<sge::font::ttf_size>(
					current_font,
					sge::parse::json::path(
						FCPPT_TEXT("size")));

			fcppt::container::ptr::push_back_unique_ptr(
				objects_,
				_font_system.create_font(
					sge::font::parameters()
						.family(
							real_name)
						.ttf_size(
							font_size)));

			to_object_.insert(
				std::make_pair(
					current_identifier,
					boost::prior(
						objects_.end())));
		}
		else
		{
			// string -> (objects_it,drawer_it)
			bitmap_font_name_to_object::const_iterator cached_value =
				bitmap_font_name_to_object_.find(
					name);

			if(cached_value == bitmap_font_name_to_object_.end())
			{
				fcppt::container::ptr::push_back_unique_ptr(
					objects_,
					sge::font::bitmap::create(
						_base_path.get() / FCPPT_TEXT("fonts") / name,
						_image_loader));

				cached_value =
					bitmap_font_name_to_object_.insert(
						std::make_pair(
							name,
							boost::prior(
								objects_.end()))).first;
			}

			to_object_.insert(
				std::make_pair(
					current_identifier,
					cached_value->second));
		}
	}
}

sge::font::object &
fruitlib::font::cache::get(
	fruitlib::font::identifier const &_identifier)
{
	to_object::const_iterator i =
		to_object_.find(
			_identifier);

	FCPPT_ASSERT_PRE_MESSAGE(
		i != to_object_.end(),
		FCPPT_TEXT("Font called ")+
		_identifier.get()+
		FCPPT_TEXT(" not found!"));

	return
		*(i->second);
}

fruitlib::font::cache::~cache()
{
}
