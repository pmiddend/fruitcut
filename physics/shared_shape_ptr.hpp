#ifndef FRUITCUT_PHYSICS_SHARED_SHAPE_PTR_HPP_INCLUDED
#define FRUITCUT_PHYSICS_SHARED_SHAPE_PTR_HPP_INCLUDED

#include <fcppt/tr1/memory.hpp>

class btCollisionShape;

namespace fruitcut
{
namespace physics
{
typedef
std::tr1::shared_ptr
<
	btCollisionShape
>
shared_shape_ptr;
}
}

#endif
