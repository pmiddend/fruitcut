#include "blur_filter.hpp"
#include "screen_vf/create_quad.hpp"
#include "screen_vf/format.hpp"
#include "../media_path.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/variable.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/variable_type.hpp>
#include <sge/shader/object.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <fcppt/assign/make_array.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <cstddef>

fruitcut::sandbox::blur_filter::blur_filter(
	sge::renderer::device_ptr const _renderer,
	sge::renderer::dim2 const &texture_size)
:
	renderer_(
		_renderer),
	textures_(
		fcppt::assign::make_array<sge::renderer::texture_ptr>
			(renderer_->create_texture(
					texture_size,
					sge::image::color::format::rgb8,
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none))
			(renderer_->create_texture(
					texture_size,
					sge::image::color::format::rgb8,
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none))),
	targets_(
		fcppt::assign::make_array<sge::renderer::target_ptr>
			(renderer_->create_target(
				textures_[1],
				sge::renderer::no_depth_stencil_texture()))
			(renderer_->create_target(
				textures_[0],
				sge::renderer::no_depth_stencil_texture()))),
	shaders_(
		fcppt::assign::make_array<sge::shader::object_ptr>
			(sge::shader::object_ptr(
				new sge::shader::object(
					renderer_,
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("bloom")
						/FCPPT_TEXT("blur_vertical_vertex.glsl"),
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("bloom")
						/FCPPT_TEXT("blur_vertical_fragment.glsl"),
					sge::shader::vf_to_string<screen_vf::format>(),
					fcppt::assign::make_container<sge::shader::variable_sequence>(
						sge::shader::variable(
							"texture_size",
							sge::shader::variable_type::const_,
							fcppt::math::dim::structure_cast<sge::renderer::vector2>(
								texture_size))),
					fcppt::assign::make_container<sge::shader::sampler_sequence>(
						sge::shader::sampler(
							"texture",
							textures_[0])))))
			(sge::shader::object_ptr(
				new sge::shader::object(
					renderer_,
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("bloom")
						/FCPPT_TEXT("blur_horizontal_vertex.glsl"),
					media_path()
						/FCPPT_TEXT("shaders")
						/FCPPT_TEXT("bloom")
						/FCPPT_TEXT("blur_horizontal_fragment.glsl"),
					sge::shader::vf_to_string<screen_vf::format>(),
					fcppt::assign::make_container<sge::shader::variable_sequence>(
						sge::shader::variable(
							"texture_size",
							sge::shader::variable_type::const_,
							fcppt::math::dim::structure_cast<sge::renderer::vector2>(
								texture_size))),
					fcppt::assign::make_container<sge::shader::sampler_sequence>(
						sge::shader::sampler(
							"texture",
							textures_[1])))))),
	quads_(
		fcppt::assign::make_array<sge::renderer::vertex_buffer_ptr>
			(screen_vf::create_quad(
				*shaders_[0],
				renderer_))
			(screen_vf::create_quad(
				*shaders_[1],
				renderer_)))
{
}

void
fruitcut::sandbox::blur_filter::render(
	render_callback const &_render)
{
	sge::renderer::scoped_target const target_(
		renderer_,
		targets_[1]); 

	sge::renderer::scoped_block const block_(
		renderer_);

	_render();
}

void
fruitcut::sandbox::blur_filter::iterate()
{
	for(std::size_t i = 0; i < static_cast<std::size_t>(shaders_.size()); ++i)
	{
		sge::shader::scoped scoped_shader(
			*shaders_[i]);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quads_[i]);

		sge::renderer::scoped_target const target_(
			renderer_,
			targets_[i]); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quads_[i]->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}

sge::renderer::texture_ptr const
fruitcut::sandbox::blur_filter::texture() const
{
	return textures_[0];
}

fruitcut::sandbox::blur_filter::~blur_filter()
{
}
