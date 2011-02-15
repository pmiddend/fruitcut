#ifndef FRUITCUT_FONT_PARTICLE_UNIQUE_BASE_PTR_HPP_INCLUDED
#define FRUITCUT_FONT_PARTICLE_UNIQUE_BASE_PTR_HPP_INCLUDED

#include "base_fwd.hpp"
#include <fcppt/unique_ptr.hpp>

namespace fruitcut
{
namespace font
{
namespace particle
{
typedef
fcppt::unique_ptr<base>
unique_base_ptr;
}
}
}

#endif
