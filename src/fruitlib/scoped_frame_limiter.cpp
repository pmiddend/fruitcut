#include <fruitlib/scoped_frame_limiter.hpp>
#include <fcppt/time/sleep.hpp>
#include <fcppt/time/sleep_duration.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <fcppt/config/external_end.hpp>


fruitlib::scoped_frame_limiter::scoped_frame_limiter(
	fps_type const _desired_fps)
:
	before_frame_(
		clock_type::now()),
	minimum_frame_length_(
		boost::chrono::duration_cast<clock_type::duration>(
			boost::chrono::seconds(1)) / _desired_fps)
{
}

fruitlib::scoped_frame_limiter::~scoped_frame_limiter()
{
	clock_type::duration const diff =
		clock_type::now() - before_frame_;

	if(diff > minimum_frame_length_)
		return;

	fcppt::time::sleep(
		boost::chrono::duration_cast<fcppt::time::sleep_duration>(
			minimum_frame_length_ - diff));
}
