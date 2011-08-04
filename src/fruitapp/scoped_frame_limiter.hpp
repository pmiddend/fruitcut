#ifndef FRUITAPP_SCOPED_FRAME_LIMITER_HPP_INCLUDED
#define FRUITAPP_SCOPED_FRAME_LIMITER_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/chrono/high_resolution_clock.hpp>
#include <fcppt/chrono/time_point_impl.hpp>
#include <fcppt/chrono/milliseconds.hpp>

namespace fruitapp
{
class scoped_frame_limiter
{
FCPPT_NONCOPYABLE(
	scoped_frame_limiter);
public:
	typedef
	fcppt::chrono::milliseconds::rep
	numeric_type;
	
	explicit
	scoped_frame_limiter(
		fcppt::chrono::milliseconds::rep const desired_fps);

	~scoped_frame_limiter();
private:
	fcppt::chrono::milliseconds::rep const desired_fps_;
	fcppt::chrono::high_resolution_clock::time_point const before_frame_;
};
}

#endif
