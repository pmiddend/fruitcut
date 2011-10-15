#ifndef FRUITAPP_SCOPED_SCENE_ACTIVATION_HPP_INCLUDED
#define FRUITAPP_SCOPED_SCENE_ACTIVATION_HPP_INCLUDED

#include <fruitapp/scene_fwd.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
class scoped_scene_activation
{
FCPPT_NONCOPYABLE(
	scoped_scene_activation);
public:
	explicit
	scoped_scene_activation(
		scene &,
		bool);

	~scoped_scene_activation();
private:
	scene &scene_;
	bool old_activation_;
};
}

#endif
