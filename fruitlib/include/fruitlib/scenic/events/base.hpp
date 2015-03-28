#ifndef FRUITLIB_SCENIC_EVENTS_BASE_HPP_INCLUDED
#define FRUITLIB_SCENIC_EVENTS_BASE_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/symbol/class.hpp>

namespace fruitlib
{
namespace scenic
{
namespace events
{
class FCPPT_SYMBOL_CLASS base
{
FCPPT_NONCOPYABLE(
	base);
public:
	FRUITLIB_DETAIL_SYMBOL
	virtual
	~base();
protected:
	base();
};
}
}
}

#endif
