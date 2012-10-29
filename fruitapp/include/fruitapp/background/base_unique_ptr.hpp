#ifndef FRUITAPP_BACKGROUND_BASE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_BASE_UNIQUE_PTR_HPP_INCLUDED

#include <fcppt/unique_ptr.hpp>
#include <fruitapp/background/base_fwd.hpp>

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
