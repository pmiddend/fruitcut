#ifndef FRUITLIB_SCOPED_FRAME_LIMITER_HPP_INCLUDED
#define FRUITLIB_SCOPED_FRAME_LIMITER_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
class scoped_frame_limiter
{
FCPPT_NONCOPYABLE(
	scoped_frame_limiter);
public:
	typedef
	boost::chrono::high_resolution_clock
	clock_type;

	typedef
	clock_type::rep
	fps_type;

	explicit
	scoped_frame_limiter(
		fps_type);

	~scoped_frame_limiter();
private:
	clock_type::time_point const before_frame_;
	clock_type::duration minimum_frame_length_;
};
}

#endif
