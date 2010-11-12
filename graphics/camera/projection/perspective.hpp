#ifndef FRUITCUT_GRAPHICS_CAMERA_PROJECTION_PERSPECTIVE_HPP_INCLUDED
#define FRUITCUT_GRAPHICS_CAMERA_PROJECTION_PERSPECTIVE_HPP_INCLUDED

#include "../../../scalar.hpp"

namespace fruitcut
{
namespace graphics
{
namespace camera
{
namespace projection
{
class perspective
{
public:
	explicit
	perspective(
		scalar aspect,
		scalar fov,
		scalar near,
		scalar far);

	scalar 
	aspect() const;

	scalar 
	fov() const;

	scalar 
	near() const;

	scalar 
	far() const;
private:
	scalar aspect_,fov_,near_,far_;
};
}
}
}
}

#endif
