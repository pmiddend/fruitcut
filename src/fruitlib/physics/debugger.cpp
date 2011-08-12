#include <fruitlib/physics/debugger.hpp>
#include <fruitlib/physics/world.hpp>
#include <fruitlib/physics/structure_cast.hpp>
#include <sge/line_drawer/line.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <sge/renderer/scoped_transform.hpp>
#include <sge/image/color/rgb8.hpp>
#include <sge/image/color/init.hpp>
#include <sge/camera/object.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <iostream>
#include <ostream>

fruitlib::physics::debugger::debugger(
	world &_world,
	sge::renderer::device &_renderer,
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
		renderer_),
	scoped_lock_()
{
	FCPPT_ASSERT(
		!world_.handle().getDebugDrawer());
	world_.handle().setDebugDrawer(
		this);
}

void
fruitlib::physics::debugger::update()
{
	scoped_lock_.take(
		fcppt::make_unique_ptr<sge::line_drawer::scoped_lock>(
			fcppt::ref(
				line_drawer_)));

	scoped_lock_->value().clear();

	if (debug_mode_ != btIDebugDraw::DBG_NoDebug)
		world_.handle().debugDrawWorld();

	scoped_lock_.reset();
}

void
fruitlib::physics::debugger::render()
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
fruitlib::physics::debugger::active(
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
fruitlib::physics::debugger::active() const
{
	return debug_mode_ != btIDebugDraw::DBG_NoDebug;
}

fruitlib::physics::debugger::~debugger()
{
	world_.handle().setDebugDrawer(
		0);
}

// @override
void
fruitlib::physics::debugger::drawLine(
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
fruitlib::physics::debugger::drawLine(
	btVector3 const &from,
	btVector3 const &to,
	btVector3 const &from_color,
	btVector3 const &to_color)
{
	// This MIGHT happen, for example when you use the BvhMeshShape. A
	// better solution than return; here would be to queue up those
	// triangles. TODO
	if(!scoped_lock_)
		return;
	scoped_lock_->value().push_back(
		sge::line_drawer::line(
			structure_cast<sge::renderer::vector3>(
				from),
			structure_cast<sge::renderer::vector3>(
				to),
			sge::image::color::any::object(
				sge::image::color::rgb8(
					(sge::image::color::init::red %= from_color.getX())
					(sge::image::color::init::green %= from_color.getY())
					(sge::image::color::init::blue %= from_color.getZ()))),
			sge::image::color::any::object(
				sge::image::color::rgb8(
					(sge::image::color::init::red %= to_color.getX())
					(sge::image::color::init::green %= to_color.getY())
					(sge::image::color::init::blue %= to_color.getZ())))));
}

// @override
void
fruitlib::physics::debugger::drawContactPoint(
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
fruitlib::physics::debugger::draw3dText(
	btVector3 const&,
	char const*)
{
}

// @override
void
fruitlib::physics::debugger::reportErrorWarning(
	char const* warningString)
{
	std::cerr
		<< "Debug drawer warning: "
		<< warningString
		<< "\n";
}

// @override
void
fruitlib::physics::debugger::setDebugMode(
	int const _debug_mode)
{
	debug_mode_ = _debug_mode;
}

// @override
int
fruitlib::physics::debugger::getDebugMode() const
{
	return debug_mode_;
}
