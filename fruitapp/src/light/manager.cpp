#include <fruitapp/light/directional_light_source_from_json.hpp>
#include <fruitapp/light/manager.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/path.hpp>
#include <fcppt/make_unique_ptr_fcppt.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>


fruitapp::light::manager::manager(
	sge::parse::json::object const &_json)
:
	directional_source_(
		fcppt::make_unique_ptr_fcppt<fruitapp::light::directional_light_source>(
			fruitapp::light::directional_light_source_from_json(
				sge::parse::json::find_and_convert_member<sge::parse::json::object>(
					_json,
					sge::parse::json::path(
						fcppt::string(
							FCPPT_TEXT("directional-source"))))))),
	ambient_intensity_(
		sge::parse::json::find_and_convert_member<fruitapp::light::ambient_intensity::value_type>(
			_json,
			sge::parse::json::path(
				fcppt::string(
					FCPPT_TEXT("ambient-intensity")))))
{
}

fruitapp::light::directional_light_source const &
fruitapp::light::manager::directional_source() const
{
	return
		*directional_source_;
}

fruitapp::light::ambient_intensity const &
fruitapp::light::manager::ambient_intensity() const
{
	return
		ambient_intensity_;
}

fruitapp::light::manager::~manager()
{
}
