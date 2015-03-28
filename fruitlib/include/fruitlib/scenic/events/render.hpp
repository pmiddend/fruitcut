#ifndef FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_RENDER_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/scenic/events/base.hpp>
#include <sge/renderer/context/ffp_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/symbol/class.hpp>


namespace fruitlib
{
namespace scenic
{
namespace events
{
class FCPPT_SYMBOL_CLASS render
:
	public events::base
{
FCPPT_NONCOPYABLE(
	render);
public:
	FRUITLIB_DETAIL_SYMBOL
	explicit
	render(
		sge::renderer::context::ffp &);

	FRUITLIB_DETAIL_SYMBOL
	sge::renderer::context::ffp &
	context() const;

	FRUITLIB_DETAIL_SYMBOL
	~render();
private:
	sge::renderer::context::ffp &context_;
};
}
}
}

#endif
