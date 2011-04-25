#include "background.hpp"
#include "../media_path.hpp"
#include "../json/find_member.hpp"
#include <sge/image2d/file.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/active_target.hpp>
#include <sge/renderer/aspect_from_viewport.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/matrix_mode.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_transform.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/stage_type.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/dest_blend_func.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/source_blend_func.hpp>
#include <sge/renderer/state/stencil_func.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/create_planar_from_view.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/dynamic/part_index.hpp>
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/part.hpp>
#include <sge/renderer/vf/pos.hpp>
#include <sge/renderer/vf/texpos.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/viewport.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <iostream>

namespace
{
namespace vf
{
typedef 
sge::renderer::vf::pos
<
	sge::renderer::scalar,
	2
> 
position;

typedef 
sge::renderer::vf::texpos
<
	sge::renderer::scalar,
	2
> 
texcoord;

typedef 
sge::renderer::vf::part
<
	boost::mpl::vector2
	<
		position,
		texcoord
	>
> 
part;

typedef 
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		part
	>
> 
format;

typedef 
sge::renderer::vf::view<part> 
vertex_view;
}
}

// The background doesn't use OpenGL-3 because of...lazyness
fruitcut::app::background::background(
	sge::renderer::device_ptr _renderer,
	sge::image2d::multi_loader &_image_loader,
	sge::parse::json::object const &_config)
:
	renderer_(
		_renderer),
	texture_(
		sge::renderer::texture::create_planar_from_view(
			renderer_,
			_image_loader.load(
				media_path()
					/ 
						FCPPT_TEXT("textures")
					/ 
						json::find_member<fcppt::string>(
							_config,
							FCPPT_TEXT("textures/background")))->view(),
			sge::renderer::texture::filter::linear,
			sge::renderer::texture::address_mode2(
				sge::renderer::texture::address_mode::repeat),
			sge::renderer::resource_flags::none)),
	vertex_declaration_(
		renderer_->create_vertex_declaration(
			sge::renderer::vf::dynamic::make_format<vf::format>())),
	vb_(
		renderer_->create_vertex_buffer(
			vertex_declaration_,
			sge::renderer::vf::dynamic::part_index(
				0u),
			6,
			sge::renderer::resource_flags::none)),
	reps_(
		json::find_member<sge::renderer::scalar>(
			_config,
			FCPPT_TEXT("background-repeat")))
{
	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	// Left top
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			-1, 1));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			0,0));

	// Left bottom
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			-1,-1));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			0,1));

	// Right top
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			1,1));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			1,0));

	// Right top
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			1,1));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			1,0));

	// Left bottom
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			-1,-1));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			0,1));

	// Right bottom
	(vb_it)->set<vf::position>(
		vf::position::packed_type(
			1,-1));
	(vb_it++)->set<vf::texcoord>(
		vf::texcoord::packed_type(
			1,1));
}



void
fruitcut::app::background::viewport_changed()
{
}

fruitcut::app::background::~background()
{
}

void
fruitcut::app::background::render()
{
	sge::renderer::scoped_vertex_declaration scoped_decl(
		renderer_,
		vertex_declaration_);

	sge::renderer::scoped_vertex_buffer scoped_vb(
		renderer_,
		vb_);

	sge::renderer::pixel_rect const viewport_rect =
		renderer_->onscreen_target()->viewport().get();

	sge::renderer::scalar const aspect = 
		sge::renderer::aspect_from_viewport(
			sge::renderer::active_target(*renderer_)->viewport());

	sge::renderer::texture::scoped scoped_texture(
		renderer_,
		texture_,
		static_cast<sge::renderer::stage_type>(
			0));
	sge::renderer::scoped_transform scoped_world(
		renderer_,
		sge::renderer::matrix_mode::world,
		sge::renderer::matrix4::identity());
	sge::renderer::scoped_transform scoped_projection(
		renderer_,
		sge::renderer::matrix_mode::projection,
		sge::renderer::matrix4::identity());
	sge::renderer::scoped_transform scoped_texture_matrix(
		renderer_,
		sge::renderer::matrix_mode::texture,
		sge::renderer::matrix4(
			viewport_rect.w() > viewport_rect.h() ? reps_ : (reps_ * aspect),0,0,0,
			0,viewport_rect.w() > viewport_rect.h() ? reps_ : (reps_ * aspect),0,0,
			0,0,1,0,
			0,0,0,1));
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::bool_::enable_alpha_blending = true)
			(sge::renderer::state::source_blend_func::src_alpha)
			(sge::renderer::state::dest_blend_func::inv_src_alpha)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::depth_func::off)
			(sge::renderer::state::stencil_func::off)
			(sge::renderer::state::draw_mode::fill));
	renderer_->render(
		sge::renderer::first_vertex(0),
		sge::renderer::vertex_count(
			6),
		sge::renderer::nonindexed_primitive_type::triangle);
}

void
fruitcut::app::background::update()
{
}
