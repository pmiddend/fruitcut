#ifndef FRUITAPP_GUI_DIALOGS_RANKING_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_GUI_DIALOGS_RANKING_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/gui/dialogs/ranking_fwd.hpp>
#include <fcppt/unique_ptr.hpp>

namespace fruitapp
{
namespace gui
{
namespace dialogs
{
typedef
fcppt::unique_ptr<fruitapp::gui::dialogs::ranking>
ranking_unique_ptr;
}
}
}

#endif
