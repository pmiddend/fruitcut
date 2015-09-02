#ifndef FRUITAPP_GUI_SYSTEM_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_GUI_SYSTEM_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/gui/system_fwd.hpp>
#include <fcppt/unique_ptr_impl.hpp>


namespace fruitapp
{
namespace gui
{
typedef
fcppt::unique_ptr<fruitapp::gui::system>
system_unique_ptr;
}
}

#endif
