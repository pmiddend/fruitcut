#ifndef FRUITAPP_SCENE_HPP_INCLUDED
#define FRUITAPP_SCENE_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/renderer/context/core_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class scene
:
	public fruitlib::scenic::node<scene>
{
FCPPT_NONCOPYABLE(
	scene);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	scene(
		fruitlib::scenic::optional_parent const &);

	void
	active(
		bool);

	bool
	active() const;

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	render_children(
		sge::renderer::context::core &);

	~scene();
private:
	bool active_;
};
}

#endif
