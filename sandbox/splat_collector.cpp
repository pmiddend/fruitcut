#include "splat_collector.hpp"
#include "../sprite/parameters.hpp"
#include "../media_path.hpp"
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
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
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

void
create_quad(
	sge::renderer::device_ptr const renderer,
	sge::shader::object &shader,
	sge::renderer::vertex_buffer_ptr const vb)
{
	sge::shader::scoped scoped_shader(
		shader);
	
	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer,
		vb);

	sge::renderer::scoped_vertex_lock const vblock(
		vb,
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
	quad_(
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf_format>(),
			static_cast<sge::renderer::size_type>(
				6),
			sge::renderer::resource_flags::none)),
	copy_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_vertex.glsl"),
		media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("copy_fragment.glsl"),
		sge::shader::vf_to_string<vf_format>(),
		fcppt::assign::make_container<sge::shader::variable_sequence>(
			sge::shader::variable(
				"texture_size",
				sge::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<sge::renderer::vector2>(
					renderer_->screen_size()))),
		fcppt::assign::make_container<sge::shader::sampler_sequence>(
			sge::shader::sampler(
				"texture",
				texture_)))
{
	create_quad(
		renderer_,
		copy_shader_,
		quad_);
}

sge::renderer::texture_ptr const
fruitcut::sandbox::splat_collector::texture()
{
	return texture_;
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
