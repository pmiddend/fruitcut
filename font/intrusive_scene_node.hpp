#ifndef FRUITCUT_FONT_INTRUSIVE_SCENE_NODE_HPP_INCLUDED
#define FRUITCUT_FONT_INTRUSIVE_SCENE_NODE_HPP_INCLUDED

#include "scene_node.hpp"
#include "../scenic/nodes/intrusive.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace font
{
class intrusive_scene_node
:
	public scene_node,
	public scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	intrusive_scene_node);
public:
	explicit
	intrusive_scene_node(
		object_parameters const &,
		scenic::color const &,
		scenic::scale);

	~intrusive_scene_node();
};
}
}

#endif
