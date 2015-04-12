#include <fruitapp/exception.hpp>
#include <fruitapp/media_path.hpp>
#include <fruitapp/graphics_settings/object.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/get_exn.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/type_name.hpp>
#include <fcppt/variant/holds_type.hpp>


namespace
{
sge::parse::json::object const
create_current(
	sge::parse::json::object const &_presets,
	sge::parse::json::object const _input)
{
	fcppt::string const preset_name(
		sge::parse::json::find_member_exn<fcppt::string>(
			_input.members,
			fcppt::string(
				FCPPT_TEXT("graphics-preset"))));

	if(preset_name == fcppt::string(FCPPT_TEXT("custom")))
		return
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				_input.members,
				fcppt::string(
					FCPPT_TEXT("graphics-settings-custom")));

	sge::parse::json::member_map::const_iterator const preset_it(
		_presets.members.find(
			preset_name));

	if(preset_it == _presets.members.end())
		throw
			fruitapp::exception(
				FCPPT_TEXT("Couldn't find graphics preset ")+
				preset_name);

	return
		sge::parse::json::get_exn<sge::parse::json::object>(
			preset_it->second);
}
}

fruitapp::graphics_settings::object::object(
	sge::parse::json::object const &_configuration,
	fruitapp::config_variables::string_variable &_current_preset)
:
	presets_(
		sge::parse::json::parse_file_exn(
			fruitapp::media_path() / FCPPT_TEXT("graphics_presets.json")).object()),
	current_(
		create_current(
			presets_,
			_configuration)),
	current_preset_(
		_current_preset)
{
}

fruitapp::graphics_settings::preset_name_set const
fruitapp::graphics_settings::object::preset_names() const
{
	fruitapp::graphics_settings::preset_name_set result;

	for(
		sge::parse::json::member_map::const_iterator graphics_node_it(
			presets_.members.begin());
		graphics_node_it != presets_.members.end();
		++graphics_node_it)
		result.insert(
			fruitapp::graphics_settings::preset_identifier(
				graphics_node_it->first));

	return
		result;
}

sge::parse::json::object const &
fruitapp::graphics_settings::object::current() const
{
	return
		current_;
}

fruitapp::config_variables::string_variable const &
fruitapp::graphics_settings::object::current_preset() const
{
	return
		current_preset_;
}

fruitapp::config_variables::string_variable &
fruitapp::graphics_settings::object::current_preset()
{
	return
		current_preset_;
}

fruitapp::graphics_settings::object::~object()
{
}
