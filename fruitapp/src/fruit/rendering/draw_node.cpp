#include <fruitapp/fruit/rendering/base.hpp>
#include <fruitapp/fruit/rendering/base_unique_ptr.hpp>
#include <fruitapp/fruit/rendering/cg.hpp>
#include <fruitapp/fruit/rendering/draw_node.hpp>
#include <fruitapp/fruit/rendering/ffp.hpp>
#include <fruitapp/light/manager.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/string_to_path.hpp>
#include <sge/renderer/context/ffp.hpp>
#include <sge/renderer/device/ffp.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>


fruitapp::fruit::rendering::draw_node::draw_node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::renderer::device::core &_renderer,
	sge::parse::json::object const &_configuration,
	sge::shader::optional_context_ref const &_shader_context,
	fruitapp::fruit::manager const &_fruit_manager,
	sge::camera::base const &_camera,
	fruitapp::light::manager const &_light_manager)
:
	node_base(
		_parent),
	implementation_(
		sge::parse::json::find_and_convert_member<bool>(
			_configuration,
			sge::parse::json::string_to_path(
				FCPPT_TEXT("graphics/fruits-use-ffp"))) ||
		!_shader_context.has_value()
		?
			fruitapp::fruit::rendering::base_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::fruit::rendering::ffp>(
					fcppt::ref(
						dynamic_cast<sge::renderer::device::ffp &>(
							_renderer)),
					fcppt::cref(
						_fruit_manager),
					fcppt::cref(
						_camera),
					fcppt::cref(
						_light_manager)))
		:
			fruitapp::fruit::rendering::base_unique_ptr(
				fcppt::make_unique_ptr<fruitapp::fruit::rendering::cg>(
					fcppt::ref(
						*_shader_context),
					fcppt::cref(
						_fruit_manager),
					fcppt::cref(
						_camera),
					fcppt::cref(
						_light_manager))))
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
