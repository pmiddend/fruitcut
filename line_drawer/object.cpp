#include "object.hpp"
#include "vf/format.hpp"
#include "vf/position.hpp"
#include "vf/vertex_view.hpp"
#include "vf/color.hpp"
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_texture.hpp>
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
#include <fcppt/math/matrix/orthogonal.hpp>
#include <fcppt/math/vector/output.hpp>
#include <boost/foreach.hpp>
#include <iostream>

namespace
{
class scoped_transform
{
FCPPT_NONCOPYABLE(
	scoped_transform);
public:
	explicit
	scoped_transform(
		sge::renderer::device_ptr const _renderer,
		sge::renderer::matrix4 const &new_world,
		sge::renderer::matrix4 const &new_projection)
	:
		renderer_(
			_renderer),
		old_world_(
			renderer_->transform(
				sge::renderer::matrix_mode::world)),
		old_projection_(
			renderer_->transform(
				sge::renderer::matrix_mode::projection))
	{
		renderer_->transform(
			sge::renderer::matrix_mode::world,
			new_world);

		renderer_->transform(
			sge::renderer::matrix_mode::projection,
			new_projection);
	}

	~scoped_transform()
	{
		renderer_->transform(
			sge::renderer::matrix_mode::world,
			old_world_);

		renderer_->transform(
			sge::renderer::matrix_mode::projection,
			old_projection_);
	}
private:
	sge::renderer::device_ptr renderer_;
	sge::renderer::matrix4 old_world_,old_projection_;
};
}

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

//	if (!vb_ || vb_->size() < static_cast<sge::renderer::size_type>(lines_.size()*2))
//	{
		vb_ = 
			renderer_->create_vertex_buffer(
				sge::renderer::vf::dynamic::make_format<vf::format>(),
				static_cast<sge::renderer::size_type>(
					lines_.size()*2),
				sge::renderer::resource_flags::none);
//	}

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
		//std::cout << "vertex: " << l.begin() << "\n";
		(vb_it++)->set<vf::color>(
			l.begin_color());
		(vb_it)->set<vf::position>(
			fcppt::math::vector::structure_cast<vf::position::packed_type>(
				l.end()));
	//	std::cout << "vertex: " << l.end() << "\n";
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

	sge::renderer::scoped_texture scoped_tex(
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

	scoped_transform scoped_tr(
		renderer_,
		sge::renderer::matrix4::identity(),
		fcppt::math::matrix::orthogonal(
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				renderer_->screen_size().w()),
			static_cast<sge::renderer::scalar>(
				renderer_->screen_size().h()),
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				10)));

	render();
}

fruitcut::line_drawer::object::~object()
{
}
