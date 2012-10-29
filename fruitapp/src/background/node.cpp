#include <fruitapp/background/node.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <fruitapp/background/base_unique_ptr.hpp>
#include <fruitapp/background/cg.hpp>
#include <fruitapp/background/ffp.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/string_to_path.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/ref.hpp>

fruitapp::background::node::node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::image2d::system &_image_system,
	sge::renderer::device::core &_renderer,
	sge::parse::json::object const &_configuration,
	sge::camera::base const &_camera,
	fruitapp::projection_manager::object &_projection_manager,
	sge::shader::optional_context_ref const &_shader_context,
	fruitapp::shadow_map::optional_object_ref const &_shadow_map)
:
	node_base(
		_parent),
	background_(
		!_shader_context.has_value() ||
		sge::parse::json::find_and_convert_member<bool>(
			_configuration,
			sge::parse::json::string_to_path(
				FCPPT_TEXT("graphics/use-ffp-background")))
		?
			fruitapp::background::base_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::background::ffp>(
					fcppt::ref(
						_image_system),
					fcppt::ref(
						dynamic_cast<sge::renderer::device::ffp &>(
							_renderer)),
					fcppt::cref(
						_configuration),
					fcppt::cref(
						_camera),
					fcppt::ref(
						_projection_manager)))
		:
			fruitapp::background::base_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::background::cg>(
					fcppt::ref(
						_image_system),
					fcppt::ref(
						*_shader_context),
					_shadow_map,
					fcppt::cref(
						_configuration),
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
