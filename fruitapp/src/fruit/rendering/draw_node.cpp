#include <fruitapp/fruit/rendering/base.hpp>
#include <fruitapp/fruit/rendering/cg.hpp>
#include <fruitapp/fruit/rendering/draw_node.hpp>
#include <fruitapp/fruit/rendering/ffp.hpp>
#include <fruitapp/graphics_settings/object.hpp>
#include <fruitapp/light/manager.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/string_to_path.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <fcppt/make_unique_ptr_fcppt.hpp>
#include <fcppt/text.hpp>
#include <fcppt/unique_ptr_to_base.hpp>


fruitapp::fruit::rendering::draw_node::draw_node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device::core &_renderer,
	fruitapp::graphics_settings::object const &_graphics_settings,
	sge::shader::optional_context_ref const &_shader_context,
	fruitapp::fruit::manager const &_fruit_manager,
	sge::camera::base const &_camera,
	fruitapp::light::manager const &_light_manager)
:
	node_base(
		_parent),
	implementation_(
		sge::parse::json::find_and_convert_member<bool>(
			_graphics_settings.current(),
			sge::parse::json::string_to_path(
				FCPPT_TEXT("fruits-use-ffp"))) ||
		!_shader_context.has_value()
		?
			fcppt::unique_ptr_to_base<
				fruitapp::fruit::rendering::base
			>(
				fcppt::make_unique_ptr_fcppt<fruitapp::fruit::rendering::ffp>(
					dynamic_cast<sge::renderer::device::ffp &>(
						_renderer),
					_fruit_manager,
					_camera,
					_light_manager))
		:
			fcppt::unique_ptr_to_base<
				fruitapp::fruit::rendering::base
			>(
				fcppt::make_unique_ptr_fcppt<fruitapp::fruit::rendering::cg>(
					_shader_context.get_unsafe(), // TODO
					_fruit_manager,
					_camera,
					_light_manager)))
{
}

void
fruitapp::fruit::rendering::draw_node::react(
	fruitlib::scenic::events::render const &_render_event)
{
	implementation_->render(
		_render_event.context());
}

fruitapp::fruit::rendering::draw_node::~draw_node()
{
}
