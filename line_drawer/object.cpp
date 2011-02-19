#include "object.hpp"
#include "vf/format.hpp"
#include "vf/position.hpp"
#include "vf/vertex_view.hpp"
#include "vf/color.hpp"
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/matrix4.hpp>
// We use no_texture which returns something incompatible to what
// scoped_texture takes as argument
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/scoped_transform.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/matrix_mode.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/source_blend_func.hpp>
#include <sge/renderer/state/dest_blend_func.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/stencil_func.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/no_texture.hpp>
#include <sge/renderer/stage_type.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/matrix/orthogonal.hpp>
#include <fcppt/math/vector/output.hpp>
#include <boost/foreach.hpp>
#include <iostream>

fruitcut::line_drawer::object::object(
	sge::renderer::device_ptr const _renderer)
:
	renderer_(
		_renderer)
{
}

fruitcut::line_drawer::line_sequence &
fruitcut::line_drawer::object::lines()
{
	return lines_;
}

fruitcut::line_drawer::line_sequence const &
fruitcut::line_drawer::object::lines() const
{
	return lines_;
}

void
fruitcut::line_drawer::object::update()
{
	if (lines_.empty())
		return;

	if (!vb_ || vb_->size() < static_cast<sge::renderer::size_type>(lines_.size()*2))
		vb_ = 
			renderer_->create_vertex_buffer(
				sge::renderer::vf::dynamic::make_format<vf::format>(),
				static_cast<sge::renderer::size_type>(
					lines_.size()*2),
				sge::renderer::resource_flags::none);

	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	BOOST_FOREACH(
		line const &l,
		lines_)
	{
		(vb_it)->set<vf::position>(
			fcppt::math::vector::structure_cast<vf::position::packed_type>(
				l.begin()));
		(vb_it++)->set<vf::color>(
			l.begin_color());
		(vb_it)->set<vf::position>(
			fcppt::math::vector::structure_cast<vf::position::packed_type>(
				l.end()));
		(vb_it++)->set<vf::color>(
			l.end_color());
	}
}

void
fruitcut::line_drawer::object::render()
{
	if (!vb_ || lines_.empty())
		return;

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

	sge::renderer::scoped_vertex_buffer scoped_vb(
		renderer_,
		vb_);

	sge::renderer::texture::scoped scoped_tex(
		renderer_,
		sge::renderer::no_texture(),
		static_cast<sge::renderer::stage_type>(
			0));

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::nonindexed_primitive_type::line);
}

void
fruitcut::line_drawer::object::render_screen_space()
{
	if (!vb_ || lines_.empty())
		return;

	sge::renderer::scoped_transform projection_scope(
		renderer_,
		sge::renderer::matrix_mode::projection,
		fcppt::math::matrix::orthogonal(
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				renderer_->onscreen_target()->viewport().get().w()),
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				renderer_->onscreen_target()->viewport().get().h()),
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				10)));

	sge::renderer::scoped_transform world_scope(
		renderer_,
		sge::renderer::matrix_mode::world,
		sge::renderer::matrix4::identity());

	render();
}

fruitcut::line_drawer::object::~object()
{
}
