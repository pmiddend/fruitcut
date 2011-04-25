#include "scoped_scene_activation.hpp"
#include "scene.hpp"

fruitcut::app::scoped_scene_activation::scoped_scene_activation(
	scene &_scene,
	bool const new_activation)
:
	scene_(
		_scene),
	old_activation_(
		scene_.active())
{
	scene_.active(
		new_activation);
}

fruitcut::app::scoped_scene_activation::~scoped_scene_activation()
{
	scene_.active(
		old_activation_);
}
