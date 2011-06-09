#include "performance_timer.hpp"
#include <sge/time/time.hpp>
#include <fcppt/chrono/chrono.hpp>
#include <iostream>

fruitcut::fruitlib::performance_timer::performance_timer(
	optional_threshold const &_threshold,
	optional_callback const &_callback)
:
	threshold_(
		_threshold),
	callback_(
		_callback),
	beginning_(
		sge::time::now())
{
}

fruitcut::fruitlib::performance_timer::~performance_timer()
{
	sge::time::duration const diff = 
		sge::time::now() - beginning_;

	fcppt::chrono::milliseconds::rep const ms = 
		fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(diff).count();

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
