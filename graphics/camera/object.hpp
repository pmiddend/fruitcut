#ifndef FRUITCUT_GRAPHICS_CAMERA_OBJECT_HPP_INCLUDED
#define FRUITCUT_GRAPHICS_CAMERA_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "projection/object.hpp"

namespace fruitcut
{
namespace graphics
{
namespace camera
{
/// The camera is a class which holds a gizmo (the camera position and
/// orientation) and manipulates it via keyboard and mouse input. It
/// also offers all of the transformation matrices to be used in the
/// shaders, including the perspective matrix (the other ones you
/// could derive from the gizmo).
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit
	object(
		parameters const &);
	
	void
	update(
		scalar);
	
	mat4 const
	world() const;

	mat4 const
	rotation() const;

	mat4 const
	translation() const;

	mat4 const
	projection() const;

	mat4 const 
	mvp() const;

	math::gizmo const &
	gizmo() const;

	math::gizmo &
	gizmo();
private:
	fcppt::signal::scoped_connection key_callback_,mouse_axis_callback_;
	scalar aspect_,fov_,near_,far_;
	scalar movement_speed_,rotation_speed_;
	vec3 dirs_;
	insula::graphics::gizmo gizmo_;
	bool movement_;

	void
	key_callback(
		sge::input::keyboard::key_event const &);

	void
	mouse_axis_callback(
		sge::input::mouse::axis_event const &);
};
}
}
}

#endif
