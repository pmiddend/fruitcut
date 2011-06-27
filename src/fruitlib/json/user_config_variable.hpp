#ifndef FRUITCUT_FRUITLIB_JSON_USER_CONFIG_VARIABLE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_JSON_USER_CONFIG_VARIABLE_HPP_INCLUDED

#include "path.hpp"
#include <sge/parse/json/json.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace json
{
class user_config_variable
{
FCPPT_NONCOPYABLE(
	user_config_variable);
public:
	typedef
	fcppt::function::object<sge::parse::json::value()>
	callback;

	explicit
	user_config_variable(
		sge::parse::json::object const &global_config,
		sge::parse::json::object &user_config,
		json::path const &,
		callback const &);

	~user_config_variable();
private:
	sge::parse::json::object const &global_config_;
	sge::parse::json::object &user_config_;
	json::path const &path_;
	callback const &callback_;
};
}
}
}

#endif
