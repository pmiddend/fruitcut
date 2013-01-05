#ifndef FRUITAPP_GUI_SYSTEM_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_GUI_SYSTEM_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/gui/system_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace gui
{
typedef
std::unique_ptr<fruitapp::gui::system>
system_unique_ptr;
}
}

#endif
