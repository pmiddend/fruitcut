#ifndef FRUITAPP_BACKGROUND_BASE_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_BASE_SCOPED_PTR_HPP_INCLUDED

#include <fruitapp/background/base_fwd.hpp>
#include <fcppt/scoped_ptr.hpp>


namespace fruitapp
{
namespace background
{
typedef
fcppt::scoped_ptr<fruitapp::background::base>
base_scoped_ptr;
}
}

#endif
