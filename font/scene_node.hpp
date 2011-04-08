#ifndef FRUITCUT_FONT_SCENE_NODE_HPP_INCLUDED
#define FRUITCUT_FONT_SCENE_NODE_HPP_INCLUDED

#include "../scene/nodes/with_color.hpp"
#include "../scene/nodes/with_scale.hpp"
#include "../scene/color.hpp"
#include "object_parameters_fwd.hpp"
#include "object.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace font
{
class scene_node
:
	public scene::nodes::with_color,
	public scene::nodes::with_scale
{
FCPPT_NONCOPYABLE(
	scene_node);
public:
	explicit
	scene_node(
		object_parameters const &);

	void
	render();

	void
	update();

	fruitcut::scene::color const
	color() const;

	void
	color(
		fruitcut::scene::color const &);
private:
	fruitcut::font::object object_;
	scene::color color_;
	scene::scale scale_;
};
}
}

#endif
