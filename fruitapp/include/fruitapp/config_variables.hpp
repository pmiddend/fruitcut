#ifndef FRUITAPP_CONFIG_VARIABLES_HPP_INCLUDED
#define FRUITAPP_CONFIG_VARIABLES_HPP_INCLUDED

#include <fruitapp/fruit/area.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/config/user_config_variable.hpp>
#include <fcppt/string.hpp>


namespace fruitapp
{
class config_variables
{
FCPPT_NONCOPYABLE(
	config_variables);
public:
	typedef
	sge::parse::json::config::user_config_variable<sge::audio::scalar>
	audio_variable;

	typedef
	sge::parse::json::config::user_config_variable<fcppt::string>
	string_variable;

	config_variables(
		sge::parse::json::object const &global_config,
		sge::parse::json::object &user_config);

	audio_variable &
	music_volume();

	audio_variable &
	effects_volume();

	string_variable &
	last_user_name();

	string_variable &
	graphics_preset();

	~config_variables();
private:
	// This fixes a subtlety: The json::user_config_variables sync
	// themselves with the json tree in their destructor. Thus, if we
	// want to write the "latest" version of the json tree to a file, we
	// need to do it after the destructors of all the variables. This is
	// why we introduce this variable.
	struct destructor_write_hack
	{
	FCPPT_NONCOPYABLE(
		destructor_write_hack);
	public:
		destructor_write_hack(
			sge::parse::json::object &);

		~destructor_write_hack();

		sge::parse::json::object &user_config_file_;
	};

	destructor_write_hack write_hack_;
	audio_variable music_volume_;
	audio_variable effects_volume_;
	string_variable last_user_name_;
	string_variable graphics_preset_;
};
}

#endif
