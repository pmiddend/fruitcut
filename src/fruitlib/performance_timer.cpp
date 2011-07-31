#include <fruitlib/performance_timer.hpp>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <iostream>

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

	fcppt::chrono::milliseconds::rep const ms = 
		fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
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
