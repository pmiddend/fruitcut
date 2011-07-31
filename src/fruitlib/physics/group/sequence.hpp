#ifndef FRUITLIB_PHYSICS_GROUP_SEQUENCE_HPP_INCLUDED
#define FRUITLIB_PHYSICS_GROUP_SEQUENCE_HPP_INCLUDED

#include "object_fwd.hpp"
#include <fcppt/reference_wrapper.hpp>
#include <vector>

namespace fruitlib
{
namespace physics
{
namespace group
{
typedef
std::vector
<
	fcppt::reference_wrapper<group::object> 
>
sequence;
}
}
}

#endif
