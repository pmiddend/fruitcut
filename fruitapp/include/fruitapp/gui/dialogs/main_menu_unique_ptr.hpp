#ifndef FRUITAPP_GUI_DIALOGS_MAIN_MENU_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_MAIN_MENU_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/gui/dialogs/main_menu_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace gui
{
namespace dialogs
{
typedef
std::unique_ptr<fruitapp::gui::dialogs::main_menu>
main_menu_unique_ptr;
}
}
}

#endif
