#ifndef FRUITLIB_FONT_SCENE_NODE_HPP_INCLUDED
#define FRUITLIB_FONT_SCENE_NODE_HPP_INCLUDED

#include <fruitlib/font/object.hpp>
#include <fruitlib/font/object_parameters_fwd.hpp>
#include <fruitlib/font/scale.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


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

	scene_node(
		fruitlib::scenic::optional_parent const &,
		fruitlib::font::object_parameters const &);

	fruitlib::font::object &
	object();

	fruitlib::font::object const &
	object() const;

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	fruitlib::font::object object_;
};
}
}

#endif
