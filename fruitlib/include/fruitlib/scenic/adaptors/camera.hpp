#ifndef FRUITLIB_SCENIC_ADAPTORS_CAMERA_HPP_INCLUDED
#define FRUITLIB_SCENIC_ADAPTORS_CAMERA_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/delta/callback.hpp>
#include <fruitlib/scenic/delta/clock.hpp>
#include <fruitlib/scenic/delta/timer.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/camera/is_dynamic_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace scenic
{
namespace adaptors
{
class camera
:
	public scenic::node<camera>
{
FCPPT_NONCOPYABLE(
	camera);
public:
	typedef
	boost::mpl::vector1<scenic::events::update>
	scene_reactions;

	FRUITLIB_DETAIL_SYMBOL
	explicit
	camera(
		scenic::optional_parent const &parent,
		sge::camera::is_dynamic &,
		scenic::delta::callback const &);

	FRUITLIB_DETAIL_SYMBOL
	~camera();

	void
	react(
		scenic::events::update const &);
private:
	scenic::delta::clock clock_;
	scenic::delta::timer timer_;
	sge::camera::is_dynamic &camera_;
};
}
}
}

#endif
