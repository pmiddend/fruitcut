#include "splat_collector.hpp"
#include "../particle/sprite/parameters.hpp"
#include "../media_path.hpp"
#include "../pp/screen_vf/create_quad.hpp"
#include "../pp/screen_vf/format.hpp"
#include <sge/image/color/format.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/target_from_texture.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/texture/filter/point.hpp>
#include <sge/renderer/texture/planar_parameters.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/sprite/default_equal.hpp>
#include <sge/sprite/default_parameters.hpp>
#include <sge/sprite/defaults.hpp>
#include <sge/sprite/parameters_impl.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/part_raw.hpp>
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
		renderer_->create_planar_texture(
			sge::renderer::texture::planar_parameters(
				fcppt::math::dim::structure_cast<sge::renderer::dim2>(
					renderer_->onscreen_target()->viewport().get().dimension()),
				sge::image::color::format::rgb8,
				sge::renderer::texture::filter::point,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none))),
	temp_texture_(
		renderer_->create_planar_texture(
			sge::renderer::texture::planar_parameters(
				fcppt::math::dim::structure_cast<sge::renderer::dim2>(
					renderer_->onscreen_target()->viewport().get().dimension()),
				sge::image::color::format::rgb8,
				sge::renderer::texture::filter::point,
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none))),
	texture_target_(
		sge::renderer::target_from_texture(
			renderer_,
			texture_)),
	temp_texture_target_(
		sge::renderer::target_from_texture(
			renderer_,
			temp_texture_)),
	ss_(
		renderer_),
	background_(
		sge::sprite::default_parameters<particle::sprite::choices>()
			.texture_size()
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						temp_texture_)))
			.system(
				&ss_)
			.repetition(
				static_cast<particle::sprite::object::repetition_type>(
					1))
			.elements()),
	copy_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_fragment.glsl"),
		sge::shader::vf_to_string<pp::screen_vf::format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>
			(sge::shader::variable(
				"target_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					renderer_->onscreen_target()->viewport().get().dimension())))
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
		pp::screen_vf::create_quad(
			copy_shader_,
			renderer_))
{
}

sge::renderer::texture::planar_ptr const
fruitcut::sandbox::splat_collector::texture()
{
	return temp_texture_;
}

void
fruitcut::sandbox::splat_collector::insert(
	particle::sprite::parameters params)
{
	sprites_.push_back(
		particle::sprite::object(
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
