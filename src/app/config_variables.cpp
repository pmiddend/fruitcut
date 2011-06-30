#include "config_variables.hpp"
#include "name.hpp"
#include "../fruitlib/json/path.hpp"
#include "../fruitlib/json/output_tabbed.hpp"
#include <sge/config/config_path.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/ofstream.hpp>
#include <fcppt/text.hpp>
#include <iostream>

fruitcut::app::config_variables::destructor_write_hack::destructor_write_hack(
	sge::parse::json::object &_user_config_file)
:
	user_config_file_(
		_user_config_file)
{
}

fruitcut::app::config_variables::destructor_write_hack::~destructor_write_hack()
{
	fcppt::io::ofstream file(
		sge::config::config_path(
			app::name())/
			FCPPT_TEXT("config.json"));

	if(!file.is_open())
	{
		// Can't throw in a destructor! So just output this warning
		fcppt::io::cerr 
			<< FCPPT_TEXT("Warning: couldn't save user configuration to \"")
			<<
				fcppt::filesystem::path_to_string(
					sge::config::config_path(
						app::name())/
						FCPPT_TEXT("config.json"))
			<< FCPPT_TEXT("\" (couldn't open the file)");
		return;
	}

	file << 
		fruitlib::json::output_tabbed(
			user_config_file_);
}

fruitcut::app::config_variables::config_variables(
	sge::parse::json::object const &global_config,
	sge::parse::json::object &user_config)
:
	write_hack_(
		user_config),
	music_volume_(
		global_config,
		user_config,
		fruitlib::json::path(
			FCPPT_TEXT("music"))
			/ FCPPT_TEXT("volume")),
	effects_volume_(
		global_config,
		user_config,
		fruitlib::json::path(
			FCPPT_TEXT("effects-volume")))
{
}

fruitcut::app::config_variables::audio_variable &
fruitcut::app::config_variables::music_volume()
{
	return music_volume_;
}

fruitcut::app::config_variables::audio_variable &
fruitcut::app::config_variables::effects_volume()
{
	return effects_volume_;
}

fruitcut::app::config_variables::~config_variables()
{
}
