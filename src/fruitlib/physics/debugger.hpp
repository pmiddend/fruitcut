#ifndef FRUITLIB_PHYSICS_DEBUGGER_HPP_INCLUDED
#define FRUITLIB_PHYSICS_DEBUGGER_HPP_INCLUDED

#include <fruitlib/physics/world_fwd.hpp>
#include <sge/camera/object_fwd.hpp>
#include <sge/line_drawer/object.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <LinearMath/btIDebugDraw.h>
#include <LinearMath/btVector3.h>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitlib
{
namespace physics
{
/// bullet offers registering a debug drawer to draw contact points or
/// just shapes. This class implements this interface, caching all the
/// lines and drawing them at once (if you don't do that, it'll be
/// super-slow).
class debugger
:
	public btIDebugDraw
{
FCPPT_NONCOPYABLE(
	debugger);
public:
	explicit
	debugger(
		world &,
		sge::renderer::device &,
		sge::camera::object &);

	void
	update();

	void
	render();

	void
	active(
		bool);

	bool
	active() const;

	~debugger();
private:
	// @override
	void
	drawLine(
		btVector3 const &from,
		btVector3 const &to,
		btVector3 const &color);

	// @override
	void
	drawLine(
		btVector3 const &from,
		btVector3 const &to,
		btVector3 const &from_color,
		btVector3 const &to_color);

	// @override
	void
	drawContactPoint(
		btVector3 const &PointOnB,
		btVector3 const &normalOnB,
		btScalar distance,
		int lifeTime,
		btVector3 const & color);

	// @override
	void
	draw3dText(
		btVector3 const&location,
		char const*textString);

	// @override
	void
	reportErrorWarning(
		char const* warningString);

	// @override
	void
	setDebugMode(
		int);

	// @override
	int
	getDebugMode() const;

	/*
	void
	drawBox(
		btVector3 const& boxMin,
		btVector3 const& boxMax,
		btVector3 const& color,
		btScalar const alpha) {}

	void
	drawTriangle(
		btVector3 const& v0,
		btVector3 const& v1,
		btVector3 const& v2,
		btVector3 const &color,
		btScalar alpha) {}
	*/

	world &world_;
	sge::renderer::device &renderer_;
	sge::camera::object &camera_;
	int debug_mode_;
	sge::line_drawer::object line_drawer_;
	fcppt::scoped_ptr<sge::line_drawer::scoped_lock> scoped_lock_;
};
}
}

#endif
