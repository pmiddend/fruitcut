#include "splat_collector.hpp"
#include "../sprite/parameters.hpp"
#include "../media_path.hpp"
#include "screen_vf/create_quad.hpp"
#include "screen_vf/format.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/filter/point.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/image/color/format.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/renderer/vector2.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>

fruitcut::sandbox::splat_collector::splat_collector(
	sge::renderer::device_ptr const _renderer)
:
	renderer_(
		_renderer),
	texture_(
		renderer_->create_texture(
			fcppt::math::dim::structure_cast<sge::renderer::dim2>(
				renderer_->screen_size()),
			sge::image::color::format::rgb8,
			sge::renderer::filter::point,
			sge::renderer::resource_flags::none)),
	temp_texture_(
		renderer_->create_texture(
			fcppt::math::dim::structure_cast<sge::renderer::dim2>(
				renderer_->screen_size()),
			sge::image::color::format::rgb8,
			sge::renderer::filter::point,
			sge::renderer::resource_flags::none)),
	texture_target_(
		renderer_->create_target(
			texture_,
			sge::renderer::no_depth_stencil_texture())),
	temp_texture_target_(
		renderer_->create_target(
			temp_texture_,
			sge::renderer::no_depth_stencil_texture())),
	ss_(
		renderer_),
	background_(
		sprite::parameters()
			.texture_size()
			.visible(
				true)
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						temp_texture_)))
			.pos(
				sprite::object::point::null())
			.system(
				&ss_)
			.color(
				sprite::object::color_type(
					(sge::image::color::init::red %= 1.0)
					(sge::image::color::init::green %= 1.0)
					(sge::image::color::init::blue %= 1.0)
					(sge::image::color::init::alpha %= 1.0))).elements()),
	copy_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_fragment.glsl"),
		sge::shader::vf_to_string<screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>
			(sge::shader::variable(
				"target_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					renderer_->screen_size())))
			(sge::shader::variable(
				"flip",
				sge::shader::variable_type::const_,
				// bool not supported in sge
				static_cast<int>(
					1))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"tex",
				texture_))),
	quad_(
		screen_vf::create_quad(
			copy_shader_,
			renderer_))
{
}

sge::renderer::texture_ptr const
fruitcut::sandbox::splat_collector::texture()
{
	return temp_texture_;
}

void
fruitcut::sandbox::splat_collector::insert(
	sprite::parameters params)
{
	sprites_.push_back(
		sprite::object(
			params.system(
				&ss_).elements()));
}

void
fruitcut::sandbox::splat_collector::update()
{
	if (sprites_.size() == 0)
		return;

	// This renders using no shader, and the result is stored backwards
	{
		sge::renderer::scoped_target const target_(
			renderer_,
			texture_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		ss_.render_all(
			sge::sprite::default_equal());

		sprites_.clear();
	}

	// This uses the copy shader and copies to the temp texture (which
	// is then used by the internal sprite system, so the above call
	// uses it again)
	{
		sge::shader::scoped scoped_shader(
			copy_shader_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quad_);

		sge::renderer::scoped_target const target_(
			renderer_,
			temp_texture_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quad_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}

fruitcut::sandbox::splat_collector::~splat_collector()
{
}
