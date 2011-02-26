#ifndef FRUITCUT_LINE_DRAWER_SCOPED_LOCK_HPP_INCLUDED
#define FRUITCUT_LINE_DRAWER_SCOPED_LOCK_HPP_INCLUDED

#include "line_sequence.hpp"
#include "object_fwd.hpp"
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace line_drawer
{
class scoped_lock
{
FCPPT_NONCOPYABLE(
	scoped_lock);
public:
	explicit
	scoped_lock(
		object &);

	line_sequence &
	value() const;

	~scoped_lock();
private:
	object &object_;
};
}
}

#endif
