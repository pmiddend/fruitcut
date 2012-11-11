#include <fruitapp/postprocessing/cg/system.hpp>
#include <fruitapp/postprocessing/cg/subsystems/main.hpp>
#include <fruitapp/postprocessing/cg/subsystems/paused.hpp>
#include <fruitapp/viewport/manager.hpp>
#include <fruitlib/media_path.hpp>
#include <sge/shader/context.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/tr1/functional.hpp>


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
		_shader_context,
		fruitlib::pp::filter::base_path(
			fruitlib::media_path() / FCPPT_TEXT("shaders") / FCPPT_TEXT("postprocessing"))),
	viewport_change_connection_(
		_viewport_manager.change_callback(
			std::tr1::bind(
				&system::viewport_change,
				this,
				std::tr1::placeholders::_1),
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

	fcppt::unique_ptr<fruitapp::postprocessing::cg::subsystems::main> main_system(
		fcppt::make_unique_ptr<fruitapp::postprocessing::cg::subsystems::main>(
			fcppt::ref(
				*this),
			_parent,
			_render_callback));


	main_system_ =
		optional_main_system(
			*main_system);

	return
		fruitapp::postprocessing::subsystems::main_unique_ptr(
			fcppt::move(
				main_system));
}

fruitapp::postprocessing::subsystems::paused_unique_ptr
fruitapp::postprocessing::cg::system::create_paused_subsystem(
	fruitlib::scenic::optional_parent const &_parent)
{
	FCPPT_ASSERT_PRE(
		main_system_);

	FCPPT_ASSERT_PRE(
		!pause_active_);

	pause_active_ =
		true;

	main_system_->toggle_active();

	return
		fruitapp::postprocessing::subsystems::paused_unique_ptr(
			fcppt::make_unique_ptr<fruitapp::postprocessing::cg::subsystems::paused>(
				fcppt::ref(
					*this),
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
	FCPPT_ASSERT_PRE(
		main_system_);

	pause_active_ =
		false;

	main_system_->toggle_active();
}

fruitlib::pp::texture::counted_instance const
fruitapp::postprocessing::cg::system::main_result_texture()
{
	FCPPT_ASSERT_PRE(
		main_system_);

	return
		main_system_->result_texture();
}
