#include <fruitapp/background/base_unique_ptr.hpp>
#include <fruitapp/background/cg.hpp>
#include <fruitapp/background/ffp.hpp>
#include <fruitapp/background/node.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>


fruitapp::background::node::node(
	fruitlib::scenic::optional_parent const &_parent,
	fruitlib::texture_manager &_texture_manager,
	sge::renderer::device::core &_renderer,
	fruitapp::background::use_ffp const &_use_ffp,
	fruitapp::background::repetitions const &_repetitions,
	sge::camera::base const &_camera,
	fruitapp::projection_manager::object &_projection_manager,
	sge::shader::optional_context_ref const &_shader_context,
	fruitapp::shadow_map::optional_object_ref const &_shadow_map)
:
	node_base(
		_parent),
	background_(
		!_shader_context.has_value() ||
		_use_ffp.get()
		?
			fruitapp::background::base_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::background::ffp>(
					fcppt::ref(
						dynamic_cast<sge::renderer::device::ffp &>(
							_renderer)),
					fcppt::ref(
						_texture_manager),
					_repetitions,
					fcppt::cref(
						_camera),
					fcppt::ref(
						_projection_manager)))
		:
			fruitapp::background::base_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::background::cg>(
					fcppt::ref(
						_texture_manager),
					fcppt::ref(
						*_shader_context),
					_shadow_map,
					_repetitions,
					fcppt::cref(
						_camera),
					fcppt::ref(
						_projection_manager))))
{
}

void
fruitapp::background::node::react(
	fruitlib::scenic::events::render const &_render_event)
{
	background_->render(
		_render_event.context());
}

fruitapp::background::node::~node()
{
}
