#ifndef FRUITCUT_FRUITLIB_FONT_SCENE_NODE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_FONT_SCENE_NODE_HPP_INCLUDED

#include "color.hpp"
#include "scale.hpp"
#include "../scenic/parent_fwd.hpp"
#include "../scenic/events/render_fwd.hpp"
#include "../scenic/node.hpp"
#include "object_parameters_fwd.hpp"
#include "object.hpp"
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace font
{
class scene_node
:
	public fruitlib::scenic::node<scene_node>
{
FCPPT_NONCOPYABLE(
	scene_node);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	explicit
	scene_node(
		fruitlib::scenic::parent const &,
		object_parameters const &,
		font::color const &,
		font::scale const &);

	font::color const
	color() const;

	void
	color(
		font::color const &);

	font::scale::value_type
	scale() const;

	void
	scale(
		font::scale::value_type);

	font::object &
	object();

	font::object const &
	object() const;

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	font::object object_;
	font::color color_;
	font::scale::value_type scale_;

};
}
}
}

#endif
