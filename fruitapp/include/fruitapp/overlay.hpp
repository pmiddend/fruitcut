#ifndef FRUITAPP_OVERLAY_HPP_INCLUDED
#define FRUITAPP_OVERLAY_HPP_INCLUDED

#include <fruitapp/postprocessing/subsystems/main_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/base.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


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

	overlay(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device::ffp &,
		fruitapp::postprocessing::subsystems::main &);

	~overlay();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	sge::renderer::device::ffp &renderer_;
	fruitapp::postprocessing::subsystems::main &postprocessing_;
};
}

#endif
