#ifndef FRUITLIB_PHYSICS_DEBUGGER_HPP_INCLUDED
#define FRUITLIB_PHYSICS_DEBUGGER_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/physics/world_fwd.hpp>
#include <sge/camera/base_fwd.hpp>
#include <sge/line_drawer/object.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/context/ffp_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <LinearMath/btIDebugDraw.h>
#include <LinearMath/btVector3.h>
#include <fcppt/config/external_end.hpp>


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
	FRUITLIB_DETAIL_SYMBOL
	debugger(
		fruitlib::physics::world &,
		sge::renderer::device::ffp &,
		sge::camera::base const &);

	void
	update();

	void
	render(
		sge::renderer::context::ffp &);

	FRUITLIB_DETAIL_SYMBOL
	void
	active(
		bool);

	FRUITLIB_DETAIL_SYMBOL
	bool
	active() const;

	FRUITLIB_DETAIL_SYMBOL
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

	fruitlib::physics::world &world_;
	sge::renderer::device::ffp &renderer_;
	sge::camera::base const &camera_;
	int debug_mode_;
	sge::line_drawer::object line_drawer_;
	typedef
	fcppt::optional::object<
		fcppt::unique_ptr<
			sge::line_drawer::scoped_lock
		>
	>
	optional_scoped_lock;

	optional_scoped_lock scoped_lock_;
};
}
}

#endif
