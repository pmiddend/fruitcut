#include <fruitapp/background/base.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fruitlib/texture_manager.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <fruitapp/background/vf/format.hpp>
#include <fruitapp/background/vf/position.hpp>
#include <fruitapp/background/vf/texcoord.hpp>
#include <fruitapp/background/vf/vertex_view.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <fruitlib/math/view_plane_rect.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/context/core.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/state/core/depth_stencil/object.hpp>
#include <sge/renderer/state/core/depth_stencil/parameters.hpp>
#include <sge/renderer/state/core/depth_stencil/scoped.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/box/object.hpp>
#include <fcppt/text.hpp>
#include <fruitapp/media_path.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::background::base::~base()
{
}

fruitapp::background::base::base(
	sge::renderer::device::core &_renderer,
	fruitlib::texture_manager &_texture_manager,
	sge::camera::base const &_camera,
	fruitapp::projection_manager::object &_projection_manager,
	fruitapp::background::repetitions const &_repetitions)
:
	target_(
		_renderer.onscreen_target()),
	vertex_declaration_(
		_renderer.create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<fruitapp::background::vf::format>())),
	vb_(
		_renderer.create_vertex_buffer(
			*vertex_declaration_,
			sge::renderer::vf::dynamic::part_index(
				0u),
			sge::renderer::vertex_count(
				6u),
			sge::renderer::resource_flags_field::null())),
	texture_(
		_texture_manager.create_planar_from_path(
			fruitapp::media_path()
				/
					FCPPT_TEXT("textures")
				/
					FCPPT_TEXT("background.png"),
			sge::renderer::texture::mipmap::off(),
			sge::renderer::resource_flags_field::null())),
	projection_change_connection_(
		_projection_manager.projection_change_callback(
			std::tr1::bind(
				&base::projection_change,
				this,
				fcppt::cref(
					_camera),
				_repetitions,
				std::tr1::placeholders::_1),
			fruitapp::projection_manager::trigger_early(
				true))),
	depth_stencil_state_(
		_renderer.create_depth_stencil_state(
			sge::renderer::state::core::depth_stencil::parameters(
				sge::renderer::state::core::depth_stencil::depth::enabled(
					sge::renderer::state::core::depth_stencil::depth::func::less,
					sge::renderer::state::core::depth_stencil::depth::write_enable(
						true)),
				sge::renderer::state::core::depth_stencil::stencil::off())))
{
}

void
fruitapp::background::base::do_render(
	sge::renderer::context::core &_context)
{
	sge::renderer::scoped_vertex_declaration scoped_vertex_declaration(
		_context,
		*vertex_declaration_);

	sge::renderer::scoped_vertex_buffer scoped_vb(
		_context,
		*vb_);

	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		_context,
		*depth_stencil_state_);

	_context.render_nonindexed(
		sge::renderer::first_vertex(
			0u),
		sge::renderer::vertex_count(
			6u),
		sge::renderer::primitive_type::triangle_list);
}

sge::renderer::texture::planar &
fruitapp::background::base::texture()
{
	return
		*texture_;
}

void
fruitapp::background::base::projection_change(
	sge::camera::base const &_camera,
	fruitapp::background::repetitions const &,
	fruitlib::perspective_projection_information const &_perspective_projection_information)
{
	typedef
	fcppt::math::box::object<sge::renderer::scalar,2>
	scalar_rect;

	// zero plane because it's the visible plane located at z = 0
	scalar_rect const zero_plane(
		fruitlib::math::view_plane_rect(
			sge::camera::matrix_conversion::world_projection(
				_camera.coordinate_system(),
				_camera.projection_matrix()),
			_perspective_projection_information));

	sge::renderer::scoped_vertex_lock const vblock(
		*vb_,
		sge::renderer::lock_mode::writeonly);

	fruitapp::background::vf::vertex_view const vertices(
		vblock.value());

	fruitapp::background::vf::vertex_view::iterator vb_it(
		vertices.begin());

	sge::renderer::vector2 const repetitions(
		static_cast<sge::renderer::scalar>(
			target_.viewport().get().w()) /
		static_cast<sge::renderer::scalar>(
			texture_->size().w()),
		static_cast<sge::renderer::scalar>(
			target_.viewport().get().h()) /
		static_cast<sge::renderer::scalar>(
			texture_->size().h()));

	// Left top
	(vb_it)->set<fruitapp::background::vf::position>(
		fruitapp::background::vf::position::packed_type(
			zero_plane.left(), zero_plane.top()));
	(vb_it++)->set<fruitapp::background::vf::texcoord>(
		fruitapp::background::vf::texcoord::packed_type(
			0,0));

	// Left bottom
	(vb_it)->set<fruitapp::background::vf::position>(
		fruitapp::background::vf::position::packed_type(
			zero_plane.left(),zero_plane.bottom()));
	(vb_it++)->set<fruitapp::background::vf::texcoord>(
		fruitapp::background::vf::texcoord::packed_type(
			0,
			repetitions.y()));

	// Right top
	(vb_it)->set<fruitapp::background::vf::position>(
		fruitapp::background::vf::position::packed_type(
			zero_plane.right(),zero_plane.top()));
	(vb_it++)->set<fruitapp::background::vf::texcoord>(
		fruitapp::background::vf::texcoord::packed_type(
			repetitions.x(),0));

	// Right top
	(vb_it)->set<fruitapp::background::vf::position>(
		fruitapp::background::vf::position::packed_type(
			zero_plane.right(),zero_plane.top()));
	(vb_it++)->set<fruitapp::background::vf::texcoord>(
		fruitapp::background::vf::texcoord::packed_type(
			repetitions.x(),0));

	// Left bottom
	(vb_it)->set<fruitapp::background::vf::position>(
		fruitapp::background::vf::position::packed_type(
			zero_plane.left(),zero_plane.bottom()));
	(vb_it++)->set<fruitapp::background::vf::texcoord>(
		fruitapp::background::vf::texcoord::packed_type(
			0,repetitions.y()));

	// Right bottom
	(vb_it)->set<fruitapp::background::vf::position>(
		fruitapp::background::vf::position::packed_type(
			zero_plane.right(),zero_plane.bottom()));
	(vb_it++)->set<fruitapp::background::vf::texcoord>(
		fruitapp::background::vf::texcoord::packed_type(
			repetitions.x(),repetitions.y()));
}
