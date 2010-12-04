#include "bloom_shader.hpp"
#include "../media_path.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/pixel_pos.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/filter/point.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/default_target.hpp>
#include <sge/shader/variable_sequence.hpp>
#include <sge/shader/sampler_sequence.hpp>
#include <sge/shader/sampler.hpp>
#include <sge/shader/vf_to_string.hpp>
#include <sge/shader/scoped.hpp>
#include <sge/image/color/format.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/assign/make_container.hpp>

#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace
{
namespace tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		sge::renderer::scalar,
		2
	>,
	tags::position
> 
vf_position;

typedef 
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		vf_position
	>
> 
vf_format;

typedef 
sge::renderer::vf::view
<
	vf_format
> 
vf_vertex_view;
}

fruitcut::sandbox::bloom_shader::bloom_shader(
	sge::renderer::device_ptr const _renderer,
	render_callback const &_render_callback)
:
	renderer_(
		_renderer),
	render_callback_(
		_render_callback),
	screen_texture_(
		renderer_->create_texture(
			fcppt::math::dim::structure_cast<sge::renderer::dim2>(
				renderer_->screen_size()),
			sge::image::color::format::rgb8,
			sge::renderer::filter::point,
			sge::renderer::resource_flags::none)),
	tone_mapped_texture_(
		renderer_->create_texture(
			sge::renderer::dim2(
				256,
				256),
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	screen_target_(
		renderer_->create_target(
			screen_texture_,
			sge::renderer::no_depth_stencil_texture())),
	tone_mapped_target_(
		renderer_->create_target(
			tone_mapped_texture_,
			sge::renderer::no_depth_stencil_texture())),
	quad_vb_(
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf_format>(),
			static_cast<sge::renderer::size_type>(
				6),
			sge::renderer::resource_flags::none)),
	quad_vb2_(
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf_format>(),
			static_cast<sge::renderer::size_type>(
				6),
			sge::renderer::resource_flags::none)),
	tone_mapped_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("tone_mapping_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("tone_mapping_fragment.glsl"),
		sge::shader::vf_to_string<vf_format>(),
		sge::shader::variable_sequence(),
		//fcppt::assign::make_container<sge::shader::variable_sequence>(
		//	sge::shader::variable()),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"texture",
				screen_texture_))),
	combining_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("combining_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("bloom")/FCPPT_TEXT("combining_fragment.glsl"),
		sge::shader::vf_to_string<vf_format>(),
		sge::shader::variable_sequence(),
		fcppt::assign::make_container<sge::shader::sampler_sequence>
			(sge::shader::sampler(
				"original",
				screen_texture_))
			(sge::shader::sampler(
				"tone_mapped",
				tone_mapped_texture_)))
{
	{
	sge::shader::scoped scoped_shader(
		tone_mapped_shader_);
	
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		quad_vb_);

	sge::renderer::scoped_vertex_lock const vblock(
		quad_vb_,
		sge::renderer::lock_mode::writeonly);

	vf_vertex_view const vertices(
		vblock.value());

	vf_vertex_view::iterator vb_it(
		vertices.begin());

	// Left top
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			-1, 1));

	// Left bottom
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			-1,-1));

	// Right top
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			1,1));

	// Right top
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			1,1));

	// Left bottom
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			-1,-1));

	// Right bottom
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			1,-1));
	}

	{
	sge::shader::scoped scoped_shader(
		combining_shader_);
	
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		quad_vb2_);

	sge::renderer::scoped_vertex_lock const vblock(
		quad_vb2_,
		sge::renderer::lock_mode::writeonly);

	vf_vertex_view const vertices(
		vblock.value());

	vf_vertex_view::iterator vb_it(
		vertices.begin());

	// Left top
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			-1, 1));

	// Left bottom
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			-1,-1));

	// Right top
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			1,1));

	// Right top
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			1,1));

	// Left bottom
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			-1,-1));

	// Right bottom
	(vb_it++)->set<vf_position>(
		vf_position::packed_type(
			1,-1));
	}
}

void
fruitcut::sandbox::bloom_shader::render()
{
	{
		sge::renderer::scoped_target const target_(
			renderer_,
			screen_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		render_callback_();
	}

	{
		sge::shader::scoped scoped_shader(
			tone_mapped_shader_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quad_vb_);

		sge::renderer::scoped_target const target_(
			renderer_,
			tone_mapped_target_); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quad_vb_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}

	{
		sge::shader::scoped scoped_shader(
			combining_shader_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			quad_vb2_);

		sge::renderer::scoped_target const target_(
			renderer_,
			sge::renderer::default_target()); 

		sge::renderer::scoped_block const block_(
			renderer_);

		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				quad_vb2_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}
}

fruitcut::sandbox::bloom_shader::~bloom_shader()
{
}
