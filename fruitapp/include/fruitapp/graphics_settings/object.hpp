#ifndef FRUITAPP_GRAPHICS_SETTINGS_OBJECT_HPP_INCLUDED
#define FRUITAPP_GRAPHICS_SETTINGS_OBJECT_HPP_INCLUDED

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
	explicit
	object(
		sge::parse::json::object const &);

	fruitapp::graphics_settings::preset_name_set const
	preset_names() const
	FCPPT_PP_PURE;

	sge::parse::json::object const &
	current() const
	FCPPT_PP_PURE;

	~object();
private:
	sge::parse::json::object const presets_;
	sge::parse::json::object const current_;
};
}
}

#endif
