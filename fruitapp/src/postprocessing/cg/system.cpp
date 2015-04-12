#include <fruitapp/postprocessing/cg/system.hpp>
#include <fruitapp/postprocessing/cg/subsystems/main.hpp>
#include <fruitapp/postprocessing/cg/subsystems/paused.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <sge/shader/context.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/optional_error.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <memory>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitapp::postprocessing::cg::system::system(
	sge::shader::context &_shader_context,
	fruitapp::viewport::manager &_viewport_manager,
	sge::parse::json::object const &_configuration)
:
	configuration_(
		_configuration),
	texture_manager_(
		_shader_context.renderer()),
	filter_manager_(
		_shader_context),
	viewport_change_connection_(
		_viewport_manager.change_callback(
			std::bind(
				&system::viewport_change,
				this,
				std::placeholders::_1),
			fruitapp::viewport::trigger_early(
				true))),
	main_system_(),
	pause_active_(
		false)
{
}

fruitapp::postprocessing::subsystems::main_unique_ptr
fruitapp::postprocessing::cg::system::create_main_subsystem(
	fruitlib::scenic::optional_parent const &_parent,
	fruitapp::postprocessing::render_callback const &_render_callback)
{
	FCPPT_ASSERT_PRE(
		!main_system_.has_value());

	std::unique_ptr<fruitapp::postprocessing::cg::subsystems::main> main_system(
		fcppt::make_unique_ptr<fruitapp::postprocessing::cg::subsystems::main>(
			*this,
			_parent,
			_render_callback));


	main_system_ =
		optional_main_system(
			*main_system);

	return
		fruitapp::postprocessing::subsystems::main_unique_ptr(
			std::move(
				main_system));
}

fruitapp::postprocessing::subsystems::paused_unique_ptr
fruitapp::postprocessing::cg::system::create_paused_subsystem(
	fruitlib::scenic::optional_parent const &_parent)
{

	FCPPT_ASSERT_PRE(
		!pause_active_);

	pause_active_ =
		true;

	FCPPT_ASSERT_OPTIONAL_ERROR(
		main_system_
	).toggle_active();

	return
		fruitapp::postprocessing::subsystems::paused_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::postprocessing::cg::subsystems::paused>(
				*this,
				_parent));
}

fruitapp::postprocessing::cg::system::~system()
{
}

void
fruitapp::postprocessing::cg::system::viewport_change(
	sge::renderer::target::viewport const &)
{
	texture_manager_.clear_screen_textures();
}

void
fruitapp::postprocessing::cg::system::main_destroyed()
{
	main_system_ =
		optional_main_system();
}

void
fruitapp::postprocessing::cg::system::paused_destroyed()
{
	pause_active_ =
		false;

	FCPPT_ASSERT_OPTIONAL_ERROR(
		main_system_
	).toggle_active();
}

fruitlib::pp::texture::counted_instance const
fruitapp::postprocessing::cg::system::main_result_texture()
{
	return
		FCPPT_ASSERT_OPTIONAL_ERROR(
			main_system_
		).result_texture();
}
