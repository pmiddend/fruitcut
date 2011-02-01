#ifndef FRUITCUT_PHYSICS_DEBUG_DRAWER_HPP_INCLUDED
#define FRUITCUT_PHYSICS_DEBUG_DRAWER_HPP_INCLUDED

#include "world_fwd.hpp"
#include <sge/camera/object_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/shader/object.hpp>
#include <LinearMath/btIDebugDraw.h>
#include <LinearMath/btVector3.h>
#include <fcppt/math/vector/basic_impl.hpp>
#include <vector>
#include <utility>

namespace fruitcut
{
namespace physics
{
/// bullet offers registering a debug drawer to draw contact points or
/// just shapes. This class implements this interface, caching all the
/// lines and drawing them at once (if you don't do that, it'll be
/// super-slow).
class debug_drawer
:
	public btIDebugDraw
{
public:	
	explicit
	debug_drawer(
		world &,
		sge::renderer::device_ptr,
		sge::camera::object &);

	void
	render();

	~debug_drawer();
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
	typedef
	std::vector
	<
		std::pair
		<
			sge::renderer::vector3,
			sge::renderer::vector3	
		>
	>
	line_cache;

	world &world_;
	sge::renderer::device_ptr const renderer_;
	sge::camera::object &camera_;
	sge::shader::object shader_;
	int debug_mode_;
	line_cache lines_;
};
}
}

#endif
