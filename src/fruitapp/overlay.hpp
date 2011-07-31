#ifndef FRUITAPP_OVERLAY_HPP_INCLUDED
#define FRUITAPP_OVERLAY_HPP_INCLUDED

#include "postprocessing_fwd.hpp"
#include "../fruitlib/scenic/node.hpp"
#include "../fruitlib/scenic/optional_parent.hpp"
#include "../fruitlib/scenic/events/update_fwd.hpp"
#include "../fruitlib/scenic/events/base_fwd.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitapp
{
class overlay
:
	public fruitlib::scenic::node<overlay>
{
FCPPT_NONCOPYABLE(
	overlay);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	overlay(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device &,
		fruitapp::postprocessing &);

	~overlay();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::renderer::device &renderer_;
	fruitapp::postprocessing &postprocessing_;
};
}

#endif
