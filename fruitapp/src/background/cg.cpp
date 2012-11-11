#include <fruitapp/media_path.hpp>
#include <fruitapp/background/cg.hpp>
#include <fruitapp/shadow_map/object.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/shader/context.hpp>
#include <sge/shader/scoped_pair.hpp>
#include <fcppt/text.hpp>


fruitapp::background::cg::cg(
	fruitlib::texture_manager &_texture_manager,
	sge::shader::context &_shader_context,
	fruitapp::shadow_map::optional_object_ref const &_shadow_map,
	fruitapp::background::repetitions const &_repetitions,
	sge::camera::base const &_camera,
	fruitapp::projection_manager::object &_projection_manager)
:
	fruitapp::background::base(
		_shader_context.renderer(),
		_texture_manager,
		_camera,
		_projection_manager,
		_repetitions),
	camera_(
		_camera),
	shader_(
		_shader_context,
		*vertex_declaration_,
		sge::shader::vertex_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("background.cg")),
		sge::shader::pixel_program_path(
			fruitapp::media_path()/FCPPT_TEXT("shaders")/FCPPT_TEXT("background.cg")),
		sge::shader::optional_cflags()),
	use_shadow_map_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"use_shadow_map"),
		_shadow_map
		?
			1.0f
		:
			0.0f),
	mvp_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"mvp"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			true),
		sge::renderer::matrix4()),
	shadow_mvp_parameter_(
		shader_.vertex_program(),
		sge::shader::parameter::name(
			"shadow_mvp"),
		_shader_context.renderer(),
		sge::shader::parameter::is_projection_matrix(
			true),
		_shadow_map
		?
			_shadow_map->mvp().get()
		:
			sge::renderer::matrix4()),
	texture_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"input_texture"),
		shader_,
		_shader_context.renderer(),
		sge::shader::parameter::planar_texture::optional_value(
			fruitapp::background::base::texture())),
	shadow_map_parameter_(
		shader_.pixel_program(),
		sge::shader::parameter::name(
			"shadow_texture"),
		shader_,
		_shader_context.renderer(),
		_shadow_map
		?
			sge::shader::parameter::planar_texture::optional_value(
				_shadow_map->texture())
		:
			sge::shader::parameter::planar_texture::optional_value())
{
}

fruitapp::background::cg::~cg()
{
}

void
fruitapp::background::cg::render(
	sge::renderer::context::core &_context)
{
	sge::shader::scoped_pair scoped_shader(
		_context,
		shader_);

	mvp_parameter_.set(
		sge::camera::matrix_conversion::world_projection(
			camera_.coordinate_system(),
			camera_.projection_matrix()));

	this->do_render(
		_context);
}
