#ifndef FRUITCUT_FONT_SCENE_NODE_HPP_INCLUDED
#define FRUITCUT_FONT_SCENE_NODE_HPP_INCLUDED

#include "../scenic/nodes/with_color.hpp"
#include "../scenic/nodes/with_scale.hpp"
#include "../scenic/color.hpp"
#include "object_parameters_fwd.hpp"
#include "object.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace font
{
class scene_node
:
	public scenic::nodes::with_color,
	public scenic::nodes::with_scale
{
FCPPT_NONCOPYABLE(
	scene_node);
public:
	explicit
	scene_node(
		object_parameters const &,
		scenic::color const &,
		scenic::scale);

	void
	render();

	void
	update();

	fruitcut::scenic::color const
	color() const;

	void
	color(
		fruitcut::scenic::color const &);

	fruitcut::scenic::scale
	scale() const;

	void
	scale(
		fruitcut::scenic::scale);

	fruitcut::font::object &
	object();

	fruitcut::font::object const &
	object() const;
private:
	fruitcut::font::object object_;
	scenic::color color_;
	scenic::scale scale_;
};
}
}

#endif
