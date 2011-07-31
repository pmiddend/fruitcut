#include <fruitapp/scoped_scene_activation.hpp>
#include <fruitapp/scene.hpp>

fruitapp::scoped_scene_activation::scoped_scene_activation(
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

fruitapp::scoped_scene_activation::~scoped_scene_activation()
{
	scene_.active(
		old_activation_);
}
