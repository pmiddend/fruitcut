#ifndef FRUITAPP_GRAPHICS_SETTINGS_OBJECT_HPP_INCLUDED
#define FRUITAPP_GRAPHICS_SETTINGS_OBJECT_HPP_INCLUDED

#include <fruitapp/config_variables/string_variable.hpp>
#include <fruitapp/graphics_settings/preset_name_set.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/preprocessor/pure.hpp>


namespace fruitapp
{
namespace graphics_settings
{
class object
{
public:
	object(
		sge::parse::json::object const &,
		fruitapp::config_variables::string_variable &);

	fruitapp::graphics_settings::preset_name_set const
	preset_names() const
	FCPPT_PP_PURE;

	sge::parse::json::object const &
	current() const
	FCPPT_PP_PURE;

	fruitapp::config_variables::string_variable const &
	current_preset() const
	FCPPT_PP_PURE;

	fruitapp::config_variables::string_variable &
	current_preset()
	FCPPT_PP_PURE;

	~object();
private:
	sge::parse::json::object const presets_;
	sge::parse::json::object const current_;
	fruitapp::config_variables::string_variable &current_preset_;
};
}
}

#endif
