#include <fruitapp/fruit/rendering/shadow_node.hpp>
#include <sge/renderer/context/ffp.hpp>

fruitapp::fruit::rendering::shadow_node::shadow_node(
	fruitlib::scenic::optional_parent const &_parent,
	sge::shader::context &_shader_context,
	fruitapp::fruit::manager const &_fruit_manager,
	fruitapp::shadow_map::mvp const &_mvp)
:
	node_base(
		_parent),
	implementation_(
		_shader_context,
		_fruit_manager,
		_mvp)
{
}

fruitapp::fruit::rendering::shadow_node::~shadow_node()
{
}

void
fruitapp::fruit::rendering::shadow_node::react(
	fruitlib::scenic::events::render const &_render_event)
{
	implementation_.render(
		_render_event.context());
}
