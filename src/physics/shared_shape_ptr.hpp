#ifndef FRUITCUT_PHYSICS_SHARED_SHAPE_PTR_HPP_INCLUDED
#define FRUITCUT_PHYSICS_SHARED_SHAPE_PTR_HPP_INCLUDED

#include <fcppt/shared_ptr.hpp>

class btCollisionShape;

namespace fruitcut
{
namespace physics
{
typedef
fcppt::shared_ptr
<
	btCollisionShape
>
shared_shape_ptr;
}
}

#endif
