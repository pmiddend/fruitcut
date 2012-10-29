#ifndef FRUITAPP_POSTPROCESSING_CG_SUBSYSTEMS_MAIN_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_CG_SUBSYSTEMS_MAIN_HPP_INCLUDED

#include <fruitapp/postprocessing/render_callback.hpp>
#include <fruitapp/postprocessing/cg/system_fwd.hpp>
#include <fruitapp/postprocessing/subsystems/main.hpp>
#include <fruitlib/pp/system.hpp>
#include <fruitlib/pp/filter/add.hpp>
#include <fruitlib/pp/filter/blur.hpp>
#include <fruitlib/pp/filter/desaturate.hpp>
#include <fruitlib/pp/filter/highlight.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/filter/render_to_texture.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/renderer/context/core_fwd.hpp>
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
class main
:
	public fruitapp::postprocessing::subsystems::main,
	public fruitlib::scenic::node<main>
{
FCPPT_NONCOPYABLE(
	main);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	main(
		fruitapp::postprocessing::cg::system &,
		fruitlib::scenic::optional_parent const &,
		fruitapp::postprocessing::render_callback const &);

	void
	render_result(
		sge::renderer::context::core &);

	void
	loading_progress(
		sge::renderer::scalar);

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	toggle_active();

	sge::renderer::texture::planar_shared_ptr
	result_texture();

	~main();
private:
	fruitapp::postprocessing::cg::system &parent_system_;
	bool active_;
	fruitlib::pp::system system_;
	fruitlib::pp::filter::render_to_texture rtt_filter_;
	fruitlib::pp::filter::highlight highlight_filter_;
	fruitlib::pp::filter::blur blur_filter_;
	fruitlib::pp::filter::add add_filter_;
	fruitlib::pp::filter::desaturate desaturate_filter_;
};
}
}
}
}

#endif
