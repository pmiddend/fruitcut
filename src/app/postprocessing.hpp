#ifndef FRUITCUT_APP_POSTPROCESSING_HPP_INCLUDED
#define FRUITCUT_APP_POSTPROCESSING_HPP_INCLUDED

#include "../fruitlib/pp/system.hpp"
#include "../fruitlib/pp/texture/manager.hpp"
#include "../fruitlib/pp/filter/render_to_texture.hpp"
#include "../fruitlib/pp/filter/manager.hpp"
#include "../fruitlib/pp/filter/ssaa.hpp"
#include "../fruitlib/pp/filter/highlight.hpp"
#include "../fruitlib/pp/filter/blur.hpp"
#include "../fruitlib/pp/filter/add.hpp"
#include "../fruitlib/pp/filter/desaturate.hpp"
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/console/object_fwd.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace app
{
class postprocessing
{
public:
	explicit
	postprocessing(
		sge::renderer::device &,
		sge::console::object &,
		fcppt::function::object<void ()> const &render_func,
		sge::parse::json::object const &);

	void
	update();

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

	void
	active(
		bool);

	void
	viewport_changed();
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

	void
	list_filters(
		sge::console::arg_list const &,
		sge::console::object &);

	void
	toggle_filter(
		sge::console::arg_list const &,
		sge::console::object &);
};
}
}

#endif
