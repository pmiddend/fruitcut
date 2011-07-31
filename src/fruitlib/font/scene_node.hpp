#ifndef FRUITLIB_FONT_SCENE_NODE_HPP_INCLUDED
#define FRUITLIB_FONT_SCENE_NODE_HPP_INCLUDED

#include <fruitlib/font/color.hpp>
#include <fruitlib/font/scale.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/font/object_parameters_fwd.hpp>
#include <fruitlib/font/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

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
		fruitlib::scenic::optional_parent const &,
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

#endif
