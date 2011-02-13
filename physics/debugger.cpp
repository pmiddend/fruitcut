#include "debugger.hpp"
#include "scalar.hpp"
#include "world.hpp"
#include "structure_cast.hpp"
#include "../media_path.hpp"
#include <bullet/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/stencil_func.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/camera/object.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <ostream>

namespace
{
namespace vf_tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(tip_color)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		sge::renderer::scalar,
		3
	>,
	vf_tags::position
> 
vf_position;

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		sge::renderer::scalar,
		3
	>,
	vf_tags::tip_color
> 
vf_color;

typedef 
sge::renderer::vf::format
<
	boost::mpl::vector2
	<
		vf_position,
		vf_color
	>
> 
vertex_format;

typedef 
sge::renderer::vf::view
<
	vertex_format
> 
vertex_view;
}

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
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("debug_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("debug_fragment.glsl"),
		sge::shader::vf_to_string<vertex_format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>
			(sge::shader::variable(
					"mvp",
					sge::shader::variable_type::uniform,
					sge::renderer::matrix4())),
		sge::shader::sampler_sequence()),
	debug_mode_(
		btIDebugDraw::DBG_NoDebug), // should be zero
	lines_()
{
}

void
fruitcut::physics::debugger::render()
{
	lines_.clear();

	if (debug_mode_ != btIDebugDraw::DBG_NoDebug)
		world_.handle().debugDrawWorld();

	if (lines_.empty())
		return;

	FCPPT_ASSERT(
		lines_.size() % 2 == 0);

	sge::renderer::state::scoped const sstate(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::depth_func::off)
			(sge::renderer::state::bool_::enable_alpha_blending = false)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::stencil_func::off)
			(sge::renderer::state::draw_mode::fill));

	sge::shader::scoped scoped_shader_(
		shader_);

	shader_.set_uniform(
		FCPPT_TEXT("mvp"),
		camera_.mvp());

	sge::renderer::vertex_buffer_ptr const vb_ = 
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vertex_format>(),
			static_cast<sge::renderer::size_type>(
				lines_.size()),
			sge::renderer::resource_flags::none);

	{
		sge::renderer::scoped_vertex_lock const vblock(
			vb_,
			sge::renderer::lock_mode::writeonly);

		vertex_view const vertices(
			vblock.value());

		vertex_view::iterator vb_it(
			vertices.begin());

		BOOST_FOREACH(
			line_cache::const_reference l,
			lines_)
		{
			(vb_it)->set<vf_position>(
				fcppt::math::vector::structure_cast<vf_position::packed_type>(
					l.first));
			(vb_it++)->set<vf_color>(
				fcppt::math::vector::structure_cast<vf_color::packed_type>(
					l.second));
		}
	}

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::nonindexed_primitive_type::line);
}

void
fruitcut::physics::debugger::active(
	bool const _active)
{
	switch(
		debug_mode_)
	{
		case btIDebugDraw::DBG_DrawWireframe:
			debug_mode_ = btIDebugDraw::DBG_NoDebug;
			break;
		case btIDebugDraw::DBG_NoDebug:
			debug_mode_ = btIDebugDraw::DBG_DrawWireframe;
			break;
	}

	setDebugMode(
		debug_mode_);
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
	lines_.push_back(
		std::make_pair(
			structure_cast<sge::renderer::vector3>(
				from),
			structure_cast<sge::renderer::vector3>(
				from_color)));

	lines_.push_back(
		std::make_pair(
			structure_cast<sge::renderer::vector3>(
				to),
			structure_cast<sge::renderer::vector3>(
				to_color)));
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
		<< FCPPT_TEXT("Debug drawer warning: ") 
		<< warningString
		<< FCPPT_TEXT("\n");
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
