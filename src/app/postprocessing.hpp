#ifndef FRUITCUT_APP_POSTPROCESSING_HPP_INCLUDED
#define FRUITCUT_APP_POSTPROCESSING_HPP_INCLUDED

#include "../fruitlib/pp/system.hpp"
#include "../fruitlib/pp/texture/manager.hpp"
#include "../fruitlib/pp/filter/render_to_texture.hpp"
#include "../fruitlib/scenic/events/update_fwd.hpp"
#include "../fruitlib/scenic/optional_parent.hpp"
#include "../fruitlib/scenic/node.hpp"
#include "../fruitlib/pp/filter/manager.hpp"
#include "../fruitlib/pp/filter/ssaa.hpp"
#include "../fruitlib/pp/filter/highlight.hpp"
#include "../fruitlib/pp/filter/blur.hpp"
#include "../fruitlib/pp/filter/add.hpp"
#include "../fruitlib/pp/filter/desaturate.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/function/object.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
class postprocessing
:
	public fruitlib::scenic::node<postprocessing>
{
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::update>
	scene_reactions;

	explicit
	postprocessing(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device &,
		fcppt::function::object<void ()> const &render_func,
		sge::parse::json::object const &);

	void
	render_result();

	sge::renderer::texture::planar_ptr const
	result_texture();

	fruitlib::pp::filter::desaturate &
	desaturate_filter();

	fruitlib::pp::texture::manager &
	texture_manager();

	fruitlib::pp::filter::manager &
	filter_manager();

	bool
	active();

	void
	active(
		bool);

	void
	viewport_changed();

	void
	react(
		fruitlib::scenic::events::update const &);
private:
	fruitlib::pp::texture::manager texture_manager_;
	fruitlib::pp::filter::manager filter_manager_;
	fruitlib::pp::system system_;
	fruitlib::pp::filter::render_to_texture rtt_filter_;
	fruitlib::pp::filter::ssaa ssaa_filter_;
	fruitlib::pp::filter::highlight highlight_filter_;
	fruitlib::pp::filter::blur blur_filter_;
	fruitlib::pp::filter::add add_filter_;
	fruitlib::pp::filter::desaturate desaturate_filter_;
	fcppt::signal::scoped_connection list_connection_;
	fcppt::signal::scoped_connection toggle_connection_;
	bool active_;
};
}
}

#endif
