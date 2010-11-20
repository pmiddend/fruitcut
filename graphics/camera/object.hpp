#ifndef FRUITCUT_GRAPHICS_CAMERA_OBJECT_HPP_INCLUDED
#define FRUITCUT_GRAPHICS_CAMERA_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "projection/object.hpp"
#include "../../mat4.hpp"
#include "../../vec3.hpp"
#include "../../scalar.hpp"
#include "../../gizmo.hpp"
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>

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

	fruitcut::gizmo const &
	gizmo() const;

	fruitcut::gizmo &
	gizmo();
private:
	fcppt::signal::scoped_connection keyboard_connection_,mouse_axis_connection_;
	projection::object const projection_;
	mat4 const projection_matrix_;
	// Rotation speed means mouse sensitivity, movement-speed should be
	// self-explanatory
	scalar const movement_speed_,rotation_speed_;
	// The camera's position and orientation
	fruitcut::gizmo gizmo_;
	// Those are the directions the camera currently moves in
	// (corresponds to the movement keys currently pressed)
	vec3 dirs_;

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
