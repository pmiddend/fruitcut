#include <fruitapp/directional_light_source.hpp>
#include <fruitapp/media_path.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/rendering/ffp.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/init.hpp>
#include <sge/image/color/rgba32f.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_vertex_declaration.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_declaration.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/state/core/depth_stencil/object.hpp>
#include <sge/renderer/state/core/depth_stencil/parameters.hpp>
#include <sge/renderer/state/core/depth_stencil/scoped.hpp>
#include <sge/renderer/state/core/depth_stencil/depth/enabled.hpp>
#include <sge/renderer/state/core/depth_stencil/stencil/off.hpp>
#include <sge/renderer/state/ffp/lighting/enabled.hpp>
#include <sge/renderer/state/ffp/lighting/object.hpp>
#include <sge/renderer/state/ffp/lighting/parameters.hpp>
#include <sge/renderer/state/ffp/lighting/scoped.hpp>
#include <sge/renderer/state/ffp/lighting/light/directional.hpp>
#include <sge/renderer/state/ffp/lighting/light/object.hpp>
#include <sge/renderer/state/ffp/lighting/light/parameters.hpp>
#include <sge/renderer/state/ffp/lighting/light/scoped.hpp>
#include <sge/renderer/state/ffp/lighting/material/object.hpp>
#include <sge/renderer/state/ffp/lighting/material/object_scoped_ptr.hpp>
#include <sge/renderer/state/ffp/lighting/material/parameters.hpp>
#include <sge/renderer/state/ffp/lighting/material/scoped.hpp>
#include <sge/renderer/state/ffp/transform/object.hpp>
#include <sge/renderer/state/ffp/transform/object_scoped_ptr.hpp>
#include <sge/renderer/state/ffp/transform/parameters.hpp>
#include <sge/renderer/state/ffp/transform/scoped.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>


namespace
{
sge::image::color::any::object const
vector4_to_any_color(
	sge::renderer::vector4 const &_input)
{
	return
		sge::image::color::any::object(
			sge::image::color::rgba32f(
				(sge::image::color::init::red() = _input[0])
				(sge::image::color::init::green() = _input[1])
				(sge::image::color::init::blue() = _input[2])
				(sge::image::color::init::alpha() = _input[3])));
}

sge::renderer::state::ffp::lighting::material::parameters const
fruit_material_to_ffp_material(
	fruitapp::fruit::material::object const &_material)
{
	return
		sge::renderer::state::ffp::lighting::material::parameters(
			sge::renderer::state::ffp::lighting::diffuse_color(
				vector4_to_any_color(
					_material.diffuse_color())),
			sge::renderer::state::ffp::lighting::ambient_color(
				sge::image::colors::black()),
			sge::renderer::state::ffp::lighting::specular_color(
				vector4_to_any_color(
					_material.specular_color())),
			sge::renderer::state::ffp::lighting::material::emissive_color(
				sge::image::colors::black()),
			sge::renderer::state::ffp::lighting::material::shininess(
				_material.specular_shininess()));
}

sge::renderer::state::ffp::lighting::light::parameters const
ffp_light_from_directional_light_source(
	fruitapp::directional_light_source const &_light)
{
	return
		sge::renderer::state::ffp::lighting::light::parameters(
			sge::renderer::state::ffp::lighting::diffuse_color(
				sge::image::colors::white()),
			sge::renderer::state::ffp::lighting::specular_color(
				sge::image::colors::white()),
			sge::renderer::state::ffp::lighting::ambient_color(
				sge::image::colors::white()),
			sge::renderer::state::ffp::lighting::light::variant(
				sge::renderer::state::ffp::lighting::light::directional(
					sge::renderer::state::ffp::lighting::light::direction(
						-fcppt::math::vector::narrow_cast<sge::renderer::vector3>(
							_light.transformation()[2])))));
}
}

fruitapp::fruit::rendering::ffp::ffp(
	sge::renderer::device::ffp &_renderer,
	fruitapp::fruit::manager const &_manager,
	sge::camera::base const &_camera,
	fruitapp::directional_light_source const &_light,
	fruitapp::ambient_intensity const &_ambient_intensity)
:
	renderer_(
		_renderer),
	manager_(
		_manager),
	camera_(
		_camera),
	ambient_intensity_(
		_ambient_intensity),
	depth_stencil_state_(
		renderer_.create_depth_stencil_state(
			sge::renderer::state::core::depth_stencil::parameters(
				sge::renderer::state::core::depth_stencil::depth::enabled(
					sge::renderer::state::core::depth_stencil::depth::func::less,
					sge::renderer::state::core::depth_stencil::depth::write_enable(
						true)),
				sge::renderer::state::core::depth_stencil::stencil::off()))),
	lighting_(
		renderer_.create_lighting_state(
			sge::renderer::state::ffp::lighting::parameters(
				sge::renderer::state::ffp::lighting::enabled(
					sge::renderer::state::ffp::lighting::ambient_color(
						//sge::image::colors::black()
						vector4_to_any_color(
							sge::renderer::vector4(
								1.0f,
								1.0f,
								1.0f,
								/*
								ambient_intensity_.get(),
								ambient_intensity_.get(),
								ambient_intensity_.get(),
								*/
								1.0f))))))),
	light_(
		renderer_.create_light_state(
			ffp_light_from_directional_light_source(
				_light)))
{
}

fruitapp::fruit::rendering::ffp::~ffp()
{
}

void
fruitapp::fruit::rendering::ffp::render(
	sge::renderer::context::core &_context)
{
	sge::renderer::context::ffp &ffp_context(
		dynamic_cast<sge::renderer::context::ffp &>(
			_context));

	sge::renderer::state::ffp::lighting::scoped const scoped_lighting(
		ffp_context,
		*lighting_);

	sge::renderer::state::ffp::lighting::light::scoped const scoped_light(
		ffp_context,
		fcppt::assign::make_container<sge::renderer::state::ffp::lighting::light::const_object_ref_vector>
			(fcppt::cref(
				*light_)));

	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		ffp_context,
		*depth_stencil_state_);

	sge::renderer::state::ffp::transform::object_scoped_ptr const projection_state(
		renderer_.create_transform_state(
			sge::renderer::state::ffp::transform::parameters(
				camera_.projection_matrix().get())));

	sge::renderer::state::ffp::transform::scoped const projection_transform(
		ffp_context,
		sge::renderer::state::ffp::transform::mode::projection,
		*projection_state);

	sge::renderer::scoped_vertex_declaration scoped_vd(
		ffp_context,
		manager_.vertex_declaration());

	for(
		fruitapp::fruit::object_sequence::const_iterator i =
			manager_.fruits().begin();
		i != manager_.fruits().end();
		++i)
	{
		sge::renderer::scoped_vertex_buffer scoped_vb(
			_context,
			i->vb());

		sge::renderer::state::ffp::transform::object_scoped_ptr const world_state(
			renderer_.create_transform_state(
				sge::renderer::state::ffp::transform::parameters(
					sge::camera::matrix_conversion::world(
						camera_.coordinate_system()) *
					i->world_transform())));

		sge::renderer::state::ffp::transform::scoped const world_transform(
			ffp_context,
			sge::renderer::state::ffp::transform::mode::world,
			*world_state);

		sge::renderer::texture::scoped const scoped_texture(
			ffp_context,
			*i->prototype().texture(),
			sge::renderer::texture::stage(
				0u));

		sge::renderer::state::ffp::lighting::material::object_scoped_ptr const material_state(
			renderer_.create_material_state(
				fruit_material_to_ffp_material(
					i->prototype().material())));

		sge::renderer::state::ffp::lighting::material::scoped const scoped_material(
			ffp_context,
			*material_state);

		ffp_context.render_nonindexed(
			sge::renderer::first_vertex(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex_count(
				i->vb().size()),
			sge::renderer::primitive_type::triangle_list);
	}
}
