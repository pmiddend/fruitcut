#include <fruitapp/scoped_frame_limiter.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/time_point_arithmetic.hpp>
#include <fcppt/time/sleep_any.hpp>

fruitapp::scoped_frame_limiter::scoped_frame_limiter(
	fcppt::chrono::milliseconds::rep const _desired_fps)
:
	desired_fps_(
		_desired_fps),
	before_frame_(
		fcppt::chrono::high_resolution_clock::now())
{
}

fruitapp::scoped_frame_limiter::~scoped_frame_limiter()
{
	fcppt::chrono::milliseconds const diff = 
		fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
			fcppt::chrono::high_resolution_clock::now() - before_frame_);

	if (diff.count() < static_cast<fcppt::chrono::milliseconds::rep>(1000/desired_fps_))
		fcppt::time::sleep_any(
			fcppt::chrono::milliseconds(
				static_cast<fcppt::chrono::milliseconds::rep>(
					1000/desired_fps_ - diff.count())));
}
