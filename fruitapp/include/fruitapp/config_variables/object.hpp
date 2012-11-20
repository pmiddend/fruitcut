#ifndef FRUITAPP_CONFIG_VARIABLES_OBJECT_HPP_INCLUDED
#define FRUITAPP_CONFIG_VARIABLES_OBJECT_HPP_INCLUDED

#include <fruitapp/config_variables/audio_variable.hpp>
#include <fruitapp/config_variables/string_variable.hpp>
#include <fruitapp/config_variables/user_config_ref.hpp>
#include <fruitapp/config_variables/global_config_ref.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/parse/json/config/user_config_variable_impl.hpp>

namespace fruitapp
{
namespace config_variables
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	object(
		fruitapp::config_variables::global_config_ref const &,
		fruitapp::config_variables::user_config_ref const &);

	fruitapp::config_variables::audio_variable &
	music_volume();

	fruitapp::config_variables::audio_variable &
	effects_volume();

	fruitapp::config_variables::string_variable &
	last_user_name();

	fruitapp::config_variables::string_variable &
	graphics_preset();

	~object();
private:
	// This fixes a subtlety: The json::user_object sync
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
	fruitapp::config_variables::audio_variable music_volume_;
	fruitapp::config_variables::audio_variable effects_volume_;
	fruitapp::config_variables::string_variable last_user_name_;
	fruitapp::config_variables::string_variable graphics_preset_;
};
}
}

#endif
