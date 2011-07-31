#ifndef FRUITLIB_JSON_USER_CONFIG_VARIABLE_HPP_INCLUDED
#define FRUITLIB_JSON_USER_CONFIG_VARIABLE_HPP_INCLUDED

#include <fruitlib/json/path.hpp>
#include <fruitlib/json/modify_user_value.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/json/convert_to.hpp>
#include <sge/parse/json/json.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/signal.hpp>
#include <fcppt/noncopyable.hpp>
#include <iostream>

namespace fruitlib
{
namespace json
{
template<typename T>
class user_config_variable
{
FCPPT_NONCOPYABLE(
	user_config_variable);
public:
	typedef
	void 
	callback_fn(
		T const &);

	typedef
	fcppt::function::object<callback_fn>
	callback;

	explicit
	user_config_variable(
		sge::parse::json::object const &_global_config,
		sge::parse::json::object &_user_config,
		json::path const &_path)
	:
		global_config_(
			_global_config),
		user_config_(
			_user_config),
		path_(
			_path),
		value_(
			json::find_and_convert_member<T>(
				global_config_,
				path_)),
		callback_()
	{
	}

	void
	value(
		T const &_value)
	{
		value_ = 
			_value;
		callback_(
			value_);
	}

	T const &
	value() const
	{
		return value_;
	}

	fcppt::signal::auto_connection
	change_callback(
		callback const &f)
	{
		return 
			callback_.connect(
				f);
	}

	~user_config_variable()
	{
		json::modify_user_value(
			global_config_,
			user_config_,
			path_,
			json::convert_to(
				value_));
	}
private:
	sge::parse::json::object const &global_config_;
	sge::parse::json::object &user_config_;
	json::path const path_;
	T value_;
	fcppt::signal::object<callback_fn> callback_; 
};
}
}

#endif
