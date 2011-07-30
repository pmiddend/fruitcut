#ifndef FRUITCUT_FRUITLIB_SCENIC_EVENTS_UPDATE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_SCENIC_EVENTS_UPDATE_HPP_INCLUDED

#include "base.hpp"
#include "../update_duration.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/chrono/duration_impl.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace scenic
{
namespace events
{
class update
:
	public events::base
{
FCPPT_NONCOPYABLE(
	update);
public:
	explicit
	update(
		scenic::update_duration const &);

	scenic::update_duration const
	delta() const;

	~update();
private:
	scenic::update_duration const delta_;
};
}
}
}
}

#endif
