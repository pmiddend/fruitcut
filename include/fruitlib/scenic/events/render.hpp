#ifndef FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED

#include <fruitlib/scenic/events/base.hpp>
#include <sge/renderer/context/ffp_fwd.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace scenic
{
namespace events
{
class render
:
	public events::base
{
FCPPT_NONCOPYABLE(
	render);
public:
	explicit
	render(
		sge::renderer::context::ffp &);

	sge::renderer::context::ffp &
	context() const;

	~render();
private:
	sge::renderer::context::ffp &context_;
};
}
}
}

#endif
