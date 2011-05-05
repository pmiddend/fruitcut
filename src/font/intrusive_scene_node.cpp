#include "intrusive_scene_node.hpp"

fruitcut::font::intrusive_scene_node::intrusive_scene_node(
	object_parameters const &_params,
	scenic::color const &_color,
	scenic::scale const _scale)
:
	scene_node(
		_params,
		_color,
		_scale)
{
}

fruitcut::font::intrusive_scene_node::~intrusive_scene_node()
{
}
