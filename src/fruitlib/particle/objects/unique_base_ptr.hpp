#ifndef FRUITCUT_FRUITLIB_PARTICLE_OBJECTS_UNIQUE_BASE_PTR_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PARTICLE_OBJECTS_UNIQUE_BASE_PTR_HPP_INCLUDED

#include "base_fwd.hpp"
#include <fcppt/unique_ptr.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace particle
{
namespace objects
{
typedef
fcppt::unique_ptr<base>
unique_base_ptr;
}
}
}
}

#endif
