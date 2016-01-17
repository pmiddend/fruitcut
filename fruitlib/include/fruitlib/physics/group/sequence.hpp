#ifndef FRUITLIB_PHYSICS_GROUP_SEQUENCE_HPP_INCLUDED
#define FRUITLIB_PHYSICS_GROUP_SEQUENCE_HPP_INCLUDED

#include <fruitlib/physics/group/object_fwd.hpp>
#include <fcppt/reference.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace physics
{
namespace group
{
typedef
std::vector
<
	fcppt::reference<group::object>
>
sequence;
}
}
}

#endif
