#ifndef FRUITAPP_GRAPHICS_SETTINGS_PRESET_NAME_SET_HPP_INCLUDED
#define FRUITAPP_GRAPHICS_SETTINGS_PRESET_NAME_SET_HPP_INCLUDED

#include <fruitapp/graphics_settings/preset_identifier.hpp>
#include <fcppt/config/external_begin.hpp>
#include <set>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace graphics_settings
{
typedef
std::set<fruitapp::graphics_settings::preset_identifier>
preset_name_set;
}
}

#endif
