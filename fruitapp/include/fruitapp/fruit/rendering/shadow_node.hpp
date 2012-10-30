#ifndef FRUITAPP_FRUIT_RENDERING_SHADOW_NODE_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_SHADOW_NODE_HPP_INCLUDED

#include <fruitapp/fruit/rendering/shadow.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
namespace rendering
{
class shadow_node
:
	public fruitlib::scenic::node<shadow_node>
{
FCPPT_NONCOPYABLE(
	shadow_node);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	shadow_node(
		fruitlib::scenic::optional_parent const &,
		sge::shader::context &,
		fruitapp::fruit::manager const &,
		fruitapp::shadow_map::mvp const &);

	~shadow_node();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	fruitapp::fruit::rendering::shadow implementation_;
};
}
}
}

#endif
