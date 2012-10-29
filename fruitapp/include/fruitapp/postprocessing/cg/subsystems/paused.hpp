#ifndef FRUITAPP_POSTPROCESSING_CG_SUBSYSTEMS_PAUSED_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_CG_SUBSYSTEMS_PAUSED_HPP_INCLUDED

#include <fruitapp/postprocessing/cg/system_fwd.hpp>
#include <fruitapp/postprocessing/subsystems/paused.hpp>
#include <fruitlib/pp/system.hpp>
#include <fruitlib/pp/filter/blur.hpp>
#include <fruitlib/pp/filter/inject_texture.hpp>
#include <fruitlib/pp/filter/iterations.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/timer/basic.hpp>
#include <sge/timer/clocks/standard.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace postprocessing
{
namespace cg
{
namespace subsystems
{
class paused
:
	public fruitapp::postprocessing::subsystems::paused,
	public fruitlib::scenic::node<paused>
{
FCPPT_NONCOPYABLE(
	paused);
public:
	typedef
	boost::mpl::vector2
	<
		fruitlib::scenic::events::render,
		fruitlib::scenic::events::update
	>
	scene_reactions;

	paused(
		fruitapp::postprocessing::cg::system &,
		fruitlib::scenic::optional_parent const &);

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitlib::scenic::events::update const &);

	~paused();
private:
	fruitapp::postprocessing::cg::system &parent_system_;
	fruitlib::pp::system system_;
	fruitlib::pp::filter::inject_texture inject_texture_;
	fruitlib::pp::filter::blur blur_;
	sge::renderer::texture::planar_shared_ptr current_texture_;
	fruitlib::pp::filter::iterations blur_iterations_;
	fruitlib::pp::filter::iterations const max_blur_iterations_;
	sge::timer::basic<sge::timer::clocks::standard> blur_timer_;
};
}
}
}
}

#endif
