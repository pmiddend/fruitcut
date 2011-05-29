#ifndef FRUITCUT_APP_SCOPED_TIME_FACTOR_HPP_INCLUDED
#define FRUITCUT_APP_SCOPED_TIME_FACTOR_HPP_INCLUDED

#include "machine_fwd.hpp"
#include <sge/time/funit.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
{
class scoped_time_factor
{
FCPPT_NONCOPYABLE(
	scoped_time_factor);
public:
	explicit
	scoped_time_factor(
		machine &,
		sge::time::funit);

	~scoped_time_factor();
private:
	machine &m_;
	sge::time::funit const old_factor_;
};
}
}

#endif
