#ifndef FRUITLIB_SCENIC_ADAPTORS_LINE_DRAWER_HPP_INCLUDED
#define FRUITLIB_SCENIC_ADAPTORS_LINE_DRAWER_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/line_drawer/object_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace scenic
{
namespace adaptors
{
class line_drawer
:
	public fruitlib::scenic::node<line_drawer>
{
FCPPT_NONCOPYABLE(
	line_drawer);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	typedef
	fcppt::optional<sge::renderer::device::ffp &>
	optional_renderer;

	FRUITLIB_DETAIL_SYMBOL
	explicit
	line_drawer(
		fruitlib::scenic::optional_parent const &,
		sge::line_drawer::object &,
		optional_renderer const &);

	FRUITLIB_DETAIL_SYMBOL
	~line_drawer();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	sge::line_drawer::object &object_;
	optional_renderer const renderer_;
};
}
}
}

#endif
