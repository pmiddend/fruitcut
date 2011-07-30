#ifndef FRUITCUT_APP_OVERLAY_HPP_INCLUDED
#define FRUITCUT_APP_OVERLAY_HPP_INCLUDED

#include "../fruitlib/scenic/node.hpp"
#include "../fruitlib/scenic/parent_fwd.hpp"
#include "../fruitlib/scenic/events/render_fwd.hpp"
#include "../fruitlib/scenic/events/base_fwd.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
class overlay
:
	public fruitlib::scenic::node<overlay>
{
FCPPT_NONCOPYABLE(
	overlay);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	explicit
	overlay(
		fruitlib::scenic::parent const &,
		sge::renderer::device &);

	~overlay();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	sge::renderer::device &renderer_;
};
}
}

#endif
