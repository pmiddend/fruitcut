#ifndef FRUITAPP_GUI_DIALOGS_NAME_CHOOSER_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_NAME_CHOOSER_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/gui/dialogs/name_chooser_fwd.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitapp
{
namespace gui
{
namespace dialogs
{
typedef
fcppt::unique_ptr<fruitapp::gui::dialogs::name_chooser>
name_chooser_unique_ptr;
}
}
}

#endif
