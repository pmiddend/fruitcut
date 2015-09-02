#ifndef FRUITAPP_BACKGROUND_BASE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_BASE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/background/base_fwd.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitapp
{
namespace background
{
typedef
fcppt::unique_ptr<fruitapp::background::base>
base_unique_ptr;
}
}

#endif
