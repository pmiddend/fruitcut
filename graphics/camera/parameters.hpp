#ifndef FRUITCUT_GRAPHICS_CAMERA_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_GRAPHICS_CAMERA_PARAMETERS_HPP_INCLUDED

#include "projection/object.hpp"
#include "../../scalar.hpp"
#include "../../vec3.hpp"
#include <sge/input/keyboard/device_fwd.hpp>
#include <sge/input/mouse/device_fwd.hpp>

namespace fruitcut
{
namespace graphics
{
namespace camera
{
class parameters
{
public:
	explicit
	parameters(
		projection::object const &,
		scalar movement_speed,
		scalar rotation_speed,
		vec3 const &position,
		sge::input::keyboard::device &,
		sge::input::mouse::device &);

	projection::object const &
	projection() const;

	scalar
	movement_speed() const;

	scalar
	rotation_speed() const;

	vec3 const &
	position() const;

	sge::input::keyboard::device &
	keyboard() const;

	sge::input::mouse::device &
	mouse() const;
private:
	projection::object projection_;
	scalar movement_speed_;
	scalar rotation_speed_;
	vec3 position_;
	sge::input::keyboard::device &keyboard_;
	sge::input::mouse::device &mouse_;
};
}
}
}

#endif
