#ifndef FRUITCUT_GRAPHICS_CAMERA_PROJECTION_OBJECT_HPP_INCLUDED
#define FRUITCUT_GRAPHICS_CAMERA_PROJECTION_OBJECT_HPP_INCLUDED

#include "orthogonal.hpp"
#include "perspective.hpp"
#include <fcppt/variant/object.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace graphics
{
namespace camera
{
namespace projection
{
typedef
fcppt::variant::object
<
	boost::mpl::vector2
	<
		orthogonal,
		perspective
	>
>
object;
}
}
}
}

#endif
