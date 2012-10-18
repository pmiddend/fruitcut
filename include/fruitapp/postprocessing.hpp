#ifndef FRUITAPP_POSTPROCESSING_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_HPP_INCLUDED

#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/pp/system.hpp>
#include <fruitlib/pp/filter/add.hpp>
#include <fruitlib/pp/filter/blur.hpp>
#include <fruitlib/pp/filter/desaturate.hpp>
#include <fruitlib/pp/filter/highlight.hpp>
#include <fruitlib/pp/filter/manager.hpp>
#include <fruitlib/pp/filter/render_to_texture.hpp>
#include <fruitlib/pp/filter/ssaa.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/context/core_fwd.hpp>
#include <sge/renderer/target/viewport.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/shader/context_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class postprocessing
:
	public fruitlib::scenic::node<postprocessing>
{
FCPPT_NONCOPYABLE(
	postprocessing);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	typedef
	fcppt::function::object<void (sge::renderer::context::core &)>
	render_callback;

	postprocessing(
		fruitlib::scenic::optional_parent const &,
		sge::shader::context &,
		render_callback const &render_func,
		sge::parse::json::object const &,
		fruitapp::viewport::manager &);

	void
	render_result(
		sge::renderer::context::core &);

	sge::renderer::texture::planar_shared_ptr
	result_texture()
	FCPPT_PP_PURE;

	fruitlib::pp::filter::desaturate &
	desaturate_filter()
	FCPPT_PP_CONST;

	fruitlib::pp::texture::manager &
	texture_manager()
	FCPPT_PP_CONST;

	fruitlib::pp::filter::manager &
	filter_manager()
	FCPPT_PP_CONST;

	bool
	active()
	FCPPT_PP_PURE;

	void
	active(
		bool);

	void
	react(
		fruitlib::scenic::events::update const &);

	~postprocessing();
private:
	fruitlib::pp::texture::manager texture_manager_;
	fruitlib::pp::filter::manager filter_manager_;
	fruitlib::pp::system system_;
	fruitlib::pp::filter::render_to_texture rtt_filter_;
//	fruitlib::pp::filter::ssaa ssaa_filter_;
	fruitlib::pp::filter::highlight highlight_filter_;
	fruitlib::pp::filter::blur blur_filter_;
	fruitlib::pp::filter::add add_filter_;
	fruitlib::pp::filter::desaturate desaturate_filter_;
	bool active_;
	fcppt::signal::scoped_connection viewport_change_connection_;

	void
	viewport_change(
		sge::renderer::target::viewport const &);
};
}

#endif
