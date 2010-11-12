#ifndef FRUITCUT_GRAPHICS_CAMERA_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_GRAPHICS_CAMERA_PARAMETERS_HPP_INCLUDED

#include "projection/object.hpp"
#include "../../scalar.hpp"
#include "../../vec3.hpp"
#include "../../input/state_manager_fwd.hpp"

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
		input::state_manager &);

	projection::object const &
	projection() const;

	scalar
	movement_speed() const;

	scalar
	rotation_speed() const;

	vec3 const &
	position() const;

	input::state_manager &
	state_manager() const;
private:
	projection::object projection_;
	scalar movement_speed_;
	scalar rotation_speed_;
	vec3 position_;
	input::state_manager &state_manager_;
};
}
}
}

#endif
