#ifndef FRUITLIB_SCENIC_EVENTS_UPDATE_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_UPDATE_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/scenic/events/base.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/symbol/class.hpp>


namespace fruitlib
{
namespace scenic
{
namespace events
{
class FCPPT_SYMBOL_CLASS update
:
	public events::base
{
FCPPT_NONCOPYABLE(
	update);
public:
	FRUITLIB_DETAIL_SYMBOL
	explicit
	update();

	FRUITLIB_DETAIL_SYMBOL
	~update();
};
}
}
}

#endif
