#include <fruitlib/performance_timer.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitlib::performance_timer::performance_timer(
	optional_threshold const &_threshold,
	optional_callback const &_callback)
:
	threshold_(
		_threshold),
	callback_(
		_callback),
	beginning_(
		clock::now())
{
}

fruitlib::performance_timer::~performance_timer()
{
	clock::duration const diff =
		clock::now() - beginning_;

	boost::chrono::milliseconds::rep const ms =
		boost::chrono::duration_cast<boost::chrono::milliseconds>(
			diff).count();

	if(!threshold_ || diff > (*threshold_))
	{
		if(callback_)
			callback_(
				diff);
		std::cout
			<< "Measured "
			<< ms
			<< " ms\n";
	}
}
