#include "debugger.hpp"
#include "world.hpp"
#include "structure_cast.hpp"
#include "../line_drawer/line.hpp"
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <sge/renderer/scoped_transform.hpp>
#include <sge/image/color/rgb8.hpp>
#include <sge/image/color/init.hpp>
#include <sge/camera/object.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <iostream>
#include <ostream>

fruitcut::physics::debugger::debugger(
	world &_world,
	sge::renderer::device_ptr const _renderer,
	sge::camera::object &_camera)
:
	world_(
		_world),
	renderer_(
		_renderer),
	camera_(
		_camera),
	debug_mode_(
		btIDebugDraw::DBG_NoDebug), // should be zero
	line_drawer_(
		renderer_)
{
	world_.handle().setDebugDrawer(
		this);
}

void
fruitcut::physics::debugger::update()
{
	line_drawer_.lines().clear();

	if (debug_mode_ != btIDebugDraw::DBG_NoDebug)
		world_.handle().debugDrawWorld();

	line_drawer_.update();
}

void
fruitcut::physics::debugger::render()
{
	if (debug_mode_ == btIDebugDraw::DBG_NoDebug)
		return;

	sge::renderer::scoped_transform projection_scope(
		renderer_,
		sge::renderer::matrix_mode::projection,
		camera_.mvp());

	sge::renderer::scoped_transform world_scope(
		renderer_,
		sge::renderer::matrix_mode::world,
		sge::renderer::matrix4::identity());
	
	line_drawer_.render();
}

void
fruitcut::physics::debugger::active(
	bool const _active)
{
	setDebugMode(
		_active
		?
			btIDebugDraw::DBG_DrawWireframe/* | btIDebugDraw::DBG_DrawAabb*/
		:
			btIDebugDraw::DBG_NoDebug);
}

bool 
fruitcut::physics::debugger::active() const
{
	return debug_mode_ != btIDebugDraw::DBG_NoDebug;
}

fruitcut::physics::debugger::~debugger()
{
	world_.handle().setDebugDrawer(
		0);
}

// @override
void
fruitcut::physics::debugger::drawLine(
	btVector3 const &from,
	btVector3 const &to,
	btVector3 const &color)
{
	drawLine(
		from,
		to,
		color,
		color);
}

// @override
void
fruitcut::physics::debugger::drawLine(
	btVector3 const &from,
	btVector3 const &to,
	btVector3 const &from_color,
	btVector3 const &to_color)
{
	//std::cout << "from " << structure_cast<sge::renderer::vector3>(from) << ", to " << structure_cast<sge::renderer::vector3>(to) << "\n";
	line_drawer_.lines().push_back(
		line_drawer::line(
			structure_cast<sge::renderer::vector3>(
				from),
			structure_cast<sge::renderer::vector3>(
				to),
			sge::image::color::rgb8(
				(sge::image::color::init::red %= from_color.getX())
				(sge::image::color::init::green %= from_color.getY())
				(sge::image::color::init::blue %= from_color.getZ())),
			sge::image::color::rgb8(
				(sge::image::color::init::red %= to_color.getX())
				(sge::image::color::init::green %= to_color.getY())
				(sge::image::color::init::blue %= to_color.getZ()))));
}

// @override
void
fruitcut::physics::debugger::drawContactPoint(
	btVector3 const &point,
	btVector3 const &normal,
	btScalar distance,
	int /*lifeTime*/,
	btVector3 const &color)
{
	drawLine(
		point,
		point + normal * distance,
		color);
}

// @override
void	
fruitcut::physics::debugger::draw3dText(
	btVector3 const&,
	char const*)
{
}

// @override
void
fruitcut::physics::debugger::reportErrorWarning(
	char const* warningString)
{
	std::cerr 
		<< "Debug drawer warning: "
		<< warningString
		<< "\n";
}

// @override
void 
fruitcut::physics::debugger::setDebugMode(
	int const _debug_mode)
{
	debug_mode_ = _debug_mode;
}

// @override
int 
fruitcut::physics::debugger::getDebugMode() const
{
	return debug_mode_;
}
