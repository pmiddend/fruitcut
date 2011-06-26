#include "parse_rgba8_color.hpp"
#include "convert_from.hpp"
#include "../exception.hpp"
#include <sge/parse/json/json.hpp>
#include <sge/image/color/init.hpp>
#include <fcppt/text.hpp>
#include <vector>

sge::image::color::rgba8 const
fruitcut::fruitlib::json::parse_rgba8_color(
	sge::parse::json::value const &v)
{
	try
	{
		sge::parse::json::array const &a = 
			boost::get<sge::parse::json::array const>(
				v);

		try
		{
			std::vector<sge::parse::json::float_type> const colors = 
				json::convert_from<std::vector<sge::parse::json::float_type> >(
					a);

			return 
				sge::image::color::rgba8(
					(sge::image::color::init::red %= colors[0])
					(sge::image::color::init::green %= colors[1])
					(sge::image::color::init::blue %= colors[2])
					(sge::image::color::init::alpha %= colors[3]));
		}
		catch (boost::bad_get const &)
		{
			throw 
				exception(
					FCPPT_TEXT("The rgba color I expected didn't contain just floats (maybe you've used an integer?)!"));
		}
	}
	catch (boost::bad_get const &)
	{
		throw 
			exception(
				FCPPT_TEXT("Tried to parse an rgba color, but I have been given no array to work on!"));
	}
}
