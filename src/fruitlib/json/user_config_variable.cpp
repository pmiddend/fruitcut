#include "user_config_variable.hpp"
#include "modify_user_value.hpp"

fruitcut::fruitlib::json::user_config_variable::user_config_variable(
	sge::parse::json::object const &_global_config,
	sge::parse::json::object &_user_config,
	json::path const &_path,
	callback const &_callback)
:
	global_config_(
		_global_config),
	user_config_(
		_user_config),
	path_(
		_path),
	callback_(
		_callback)
{
}

fruitcut::fruitlib::json::user_config_variable::~user_config_variable()
{
	json::modify_user_value(
		global_config_,
		user_config_,
		path_,
		callback_());
}
