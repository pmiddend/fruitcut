#include <fruitapp/config_variables.hpp>
#include <fruitapp/name.hpp>
#include <sge/config/config_path.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/path.hpp>
#include <sge/parse/json/output/tabbed_to_string.hpp>
#include <sge/parse/json/start.hpp>
#include <fcppt/text.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/ofstream.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::config_variables::destructor_write_hack::destructor_write_hack(
	sge::parse::json::object &_user_config_file)
:
	user_config_file_(
		_user_config_file)
{
}

fruitapp::config_variables::destructor_write_hack::~destructor_write_hack()
{
	fcppt::io::ofstream file(
		sge::config::config_path(
			fruitapp::name())/
			FCPPT_TEXT("config.json"));

	if(!file.is_open())
	{
		// Can't throw in a destructor! So just output this warning
		fcppt::io::cerr()
			<< FCPPT_TEXT("Warning: couldn't save user configuration to \"")
			<<
				fcppt::filesystem::path_to_string(
					sge::config::config_path(
						fruitapp::name())/
						FCPPT_TEXT("config.json"))
			<< FCPPT_TEXT("\" (couldn't open the file)");
		return;
	}

	file <<
		sge::parse::json::output::tabbed_to_string(
			sge::parse::json::start(
				user_config_file_));
}

fruitapp::config_variables::config_variables(
	sge::parse::json::object const &global_config,
	sge::parse::json::object &user_config)
:
	write_hack_(
		user_config),
	music_volume_(
		global_config,
		user_config,
		sge::parse::json::path(
			FCPPT_TEXT("music"))
			/ FCPPT_TEXT("volume")),
	effects_volume_(
		global_config,
		user_config,
		sge::parse::json::path(
			FCPPT_TEXT("effects-volume"))),
	splatter_count_to_area_factor_(
		global_config,
		user_config,
		sge::parse::json::path(
			FCPPT_TEXT("splatter-generator"))
			/ FCPPT_TEXT("splatter-count-to-area-factor"))
{
}

fruitapp::config_variables::audio_variable &
fruitapp::config_variables::music_volume()
{
	return music_volume_;
}

fruitapp::config_variables::audio_variable &
fruitapp::config_variables::effects_volume()
{
	return effects_volume_;
}

fruitapp::config_variables::splatter_factor_variable &
fruitapp::config_variables::splatter_count_to_area_factor()
{
	return splatter_count_to_area_factor_;
}

fruitapp::config_variables::~config_variables()
{
}
