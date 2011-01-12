#ifndef FRUITCUT_APP_POSTPROCESSING_HPP_INCLUDED
#define FRUITCUT_APP_POSTPROCESSING_HPP_INCLUDED

#include "../pp/system.hpp"
#include "../pp/filter/render_to_texture.hpp"
#include "../pp/filter/ssaa.hpp"
#include "../pp/filter/highlight.hpp"
#include "../pp/filter/blur.hpp"
#include "../pp/filter/add.hpp"
#include "../pp/filter/desaturate.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/console/object_fwd.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/function.hpp>

namespace fruitcut
{
namespace app
{
class postprocessing
{
public:
	explicit
	postprocessing(
		sge::renderer::device_ptr const &,
		sge::console::object &,
		boost::function<void ()> const &render_func,
		sge::parse::json::object const &);

	void
	update();

	void
	render_result();

	pp::filter::desaturate &
	desaturate_filter();

	sge::renderer::texture_ptr const
	result_texture();

	void
	active(
		bool);
private:
	pp::system system_;
	pp::filter::render_to_texture rtt_filter_;
	pp::filter::ssaa ssaa_filter_;
	pp::filter::highlight highlight_filter_;
	pp::filter::blur blur_filter_;
	pp::filter::add add_filter_;
	pp::filter::desaturate desaturate_filter_;
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
