#include <fruitapp/media_path.hpp>
#include <fruitapp/fruit/manager.hpp>
#include <fruitapp/fruit/rendering/cg.hpp>
#include <fruitapp/fruit/rendering/sort_fruits.hpp>
#include <fruitapp/light/directional_light_source.hpp>
#include <fruitapp/light/manager.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/primitive_type.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/vector4.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <sge/renderer/state/core/depth_stencil/object.hpp>
#include <sge/renderer/state/core/depth_stencil/parameters.hpp>
#include <sge/renderer/state/core/depth_stencil/scoped.hpp>
#include <sge/renderer/state/core/depth_stencil/depth/enabled.hpp>
#include <sge/renderer/state/core/depth_stencil/stencil/off.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/renderer/texture/scoped.hpp>
#include <sge/renderer/texture/stage.hpp>
#include <sge/renderer/vertex/buffer.hpp>
#include <sge/renderer/vertex/count.hpp>
#include <sge/renderer/vertex/first.hpp>
#include <sge/renderer/vertex/scoped_buffer.hpp>
#include <sge/renderer/vertex/scoped_declaration.hpp>
#include <sge/shader/context.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/identity.hpp>
#include <fcppt/math/matrix/inverse.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/null.hpp>


fruitapp::fruit::rendering::cg::cg(
	sge::shader::context &_shader_context,
	fruitapp::fruit::manager const &_manager,
	sge::camera::base const &_camera,
	fruitapp::light::manager const &_light_manager)
:
	manager_(
		_manager),
	camera_(
		_camera),
	shader_(
		_shader_context,
		_manager.vertex_declaration(),
		sge::shader::vertex_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model.cg")),
		sge::shader::pixel_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("model.cg")),
		sge::shader::optional_cflags()),
	mvp_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"mvp"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			true),
		fcppt::math::matrix::identity<
			sge::renderer::matrix4
		>()
	),
	mv_it_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"mv_it"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			false),
		fcppt::math::matrix::identity<
			sge::renderer::matrix4
		>()
	),
	world_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"world"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			false),
		fcppt::math::matrix::identity<
			sge::renderer::matrix4
		>()
	),
	light_position_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"light_position"),
		_light_manager.directional_source().position()),
	ambient_intensity_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"ambient_intensity"),
		_light_manager.ambient_intensity().get()),
	diffuse_color_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"diffuse_color"),
		fcppt::math::vector::null<
			sge::renderer::vector4
		>()
	),
	specular_color_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"specular_color"),
		fcppt::math::vector::null<
			sge::renderer::vector4
		>()
	),
	diffuse_coefficient_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"diffuse_coefficient"),
		sge::renderer::scalar()),
	specular_coefficient_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"specular_coefficient"),
		sge::renderer::scalar()),
	specular_shininess_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"specular_shininess"),
		sge::renderer::scalar()),
	texture_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture"),
		shader_,
		_shader_context.renderer(),
		sge::shader::parameter::planar_texture::optional_value()),
	depth_stencil_state_(
		_shader_context.renderer().create_depth_stencil_state(
			sge::renderer::state::core::depth_stencil::parameters(
				sge::renderer::state::core::depth_stencil::depth::variant(
					sge::renderer::state::core::depth_stencil::depth::enabled(
						sge::renderer::state::core::depth_stencil::depth::func::less,
						sge::renderer::state::core::depth_stencil::depth::write_enable(
							true))),
				sge::renderer::state::core::depth_stencil::stencil::variant(
					sge::renderer::state::core::depth_stencil::stencil::off()))))
{
}

fruitapp::fruit::rendering::cg::~cg()
{
}

void
fruitapp::fruit::rendering::cg::render(
	sge::renderer::context::core &_context)
{
	sge::renderer::state::core::depth_stencil::scoped scoped_depth_stencil(
		_context,
		*depth_stencil_state_);

	sge::shader::scoped_pair scoped_shader(
		_context,
		shader_);

	world_parameter_.set(
		sge::camera::matrix_conversion::world(
			camera_.coordinate_system()));

	sge::renderer::vertex::scoped_declaration scoped_vd(
		_context,
		manager_.vertex_declaration());

	fruitapp::fruit::rendering::fruit_pointer_sequence fruit_pointers;
	fruitapp::fruit::rendering::sort_fruits(
		manager_.fruits(),
		fruit_pointers);

	fruitapp::fruit::prototype const *previous_prototype =
		0;

	for(
		fruitapp::fruit::rendering::fruit_pointer_sequence::const_iterator i =
			fruit_pointers.begin();
		i != fruit_pointers.end();
		++i)
	{
		sge::renderer::vertex::scoped_buffer scoped_vb(
			_context,
			(*i)->vb());

		if(&((*i)->prototype()) != previous_prototype)
		{
			texture_parameter_.set(
				sge::shader::parameter::planar_texture::optional_value(
					*(*i)->prototype().texture()));

			// Material shit
			diffuse_color_parameter_.set(
				(*i)->prototype().material().diffuse_color());

			specular_color_parameter_.set(
				(*i)->prototype().material().specular_color());

			diffuse_coefficient_parameter_.set(
				(*i)->prototype().material().diffuse_coefficient());

			specular_coefficient_parameter_.set(
				(*i)->prototype().material().specular_coefficient());

			specular_shininess_parameter_.set(
				(*i)->prototype().material().specular_shininess());

			previous_prototype =
				&((*i)->prototype());
		}

		mvp_parameter_.set(
			sge::camera::matrix_conversion::world_projection(
				camera_.coordinate_system(),
				camera_.projection_matrix()) *
			(*i)->world_transform());

		sge::renderer::matrix4 const world_transformation_matrix(
			sge::camera::matrix_conversion::world(
				camera_.coordinate_system()) *
			(*i)->world_transform());

		mv_it_parameter_.set(
			fcppt::math::matrix::transpose(
				fcppt::math::matrix::inverse(
					world_transformation_matrix)));

		_context.render_nonindexed(
			sge::renderer::vertex::first(
				static_cast<sge::renderer::size_type>(
					0)),
			sge::renderer::vertex::count(
				(*i)->vb().size()),
			sge::renderer::primitive_type::triangle_list);
	}
}
