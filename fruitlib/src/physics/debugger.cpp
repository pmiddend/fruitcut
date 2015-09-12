#include <fruitlib/physics/debugger.hpp>
#include <fruitlib/physics/structure_cast.hpp>
#include <fruitlib/physics/world.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/image/color/rgb8.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/image/color/init/blue.hpp>
#include <sge/image/color/init/green.hpp>
#include <sge/image/color/init/red.hpp>
#include <sge/line_drawer/line.hpp>
#include <sge/line_drawer/scoped_lock.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/state/ffp/transform/object.hpp>
#include <sge/renderer/state/ffp/transform/object_unique_ptr.hpp>
#include <sge/renderer/state/ffp/transform/parameters.hpp>
#include <sge/renderer/state/ffp/transform/scoped.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/maybe_void.hpp>
#include <fcppt/optional_assign.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/pre_message.hpp>
#include <fcppt/math/matrix/identity.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/config/external_begin.hpp>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <iostream>
#include <ostream>
#include <fcppt/config/external_end.hpp>


fruitlib::physics::debugger::debugger(
	fruitlib::physics::world &_world,
	sge::renderer::device::ffp &_renderer,
	sge::camera::base const &_camera)
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
	FCPPT_ASSERT_PRE_MESSAGE(
		!world_.handle().getDebugDrawer(),
		FCPPT_TEXT("You can't have two debug drawers for one world"));
	world_.handle().setDebugDrawer(
		this);
}

void
fruitlib::physics::debugger::update()
{
	{
		auto const &scoped_lock(
			fcppt::optional_assign(
				scoped_lock_,
				fcppt::make_unique_ptr<sge::line_drawer::scoped_lock>(
					line_drawer_
				)
			)
		);

		scoped_lock->value().clear();

		if (debug_mode_ != btIDebugDraw::DBG_NoDebug)
			world_.handle().debugDrawWorld();
	}

	scoped_lock_ =
		optional_scoped_lock();
}

void
fruitlib::physics::debugger::render(
	sge::renderer::context::ffp &_context)
{
	if (debug_mode_ == btIDebugDraw::DBG_NoDebug)
		return;

	sge::renderer::state::ffp::transform::object_unique_ptr const world_state(
		renderer_.create_transform_state(
			sge::renderer::state::ffp::transform::parameters(
				sge::camera::matrix_conversion::world_projection(
					camera_.coordinate_system(),
					camera_.projection_matrix()))));

	sge::renderer::state::ffp::transform::scoped const world_transform(
		_context,
		sge::renderer::state::ffp::transform::mode::world,
		*world_state);

	sge::renderer::state::ffp::transform::object_unique_ptr const projection_state(
		renderer_.create_transform_state(
			sge::renderer::state::ffp::transform::parameters(
				fcppt::math::matrix::identity<
					sge::renderer::matrix4
				>()
			)
		)
	);

	sge::renderer::state::ffp::transform::scoped const projection_transform(
		_context,
		sge::renderer::state::ffp::transform::mode::projection,
		*projection_state);

	line_drawer_.render(
		_context);
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
	fcppt::maybe_void(
		scoped_lock_,
		[
			&from,
			&to,
			&from_color,
			&to_color
		](
			auto const &_scoped_lock
		)
		{
			_scoped_lock->value().push_back(
				sge::line_drawer::line(
					fruitlib::physics::structure_cast<sge::renderer::vector3>(
						from),
					fruitlib::physics::structure_cast<sge::renderer::vector3>(
						to),
					sge::image::color::any::object(
						sge::image::color::rgb8(
							(sge::image::color::init::red() %= from_color.getX())
							(sge::image::color::init::green() %= from_color.getY())
							(sge::image::color::init::blue() %= from_color.getZ()))),
					sge::image::color::any::object(
						sge::image::color::rgb8(
							(sge::image::color::init::red() %= to_color.getX())
							(sge::image::color::init::green() %= to_color.getY())
							(sge::image::color::init::blue() %= to_color.getZ())))));
		}
	);
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
