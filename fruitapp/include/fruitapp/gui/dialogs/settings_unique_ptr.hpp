#ifndef FRUITAPP_GUI_DIALOGS_SETTINGS_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_SETTINGS_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/gui/dialogs/settings_fwd.hpp>
#include <fcppt/unique_ptr.hpp>

namespace fruitapp
{
namespace gui
{
namespace dialogs
{
typedef
fcppt::unique_ptr<fruitapp::gui::dialogs::settings>
settings_unique_ptr;
}
}
}

#endif
