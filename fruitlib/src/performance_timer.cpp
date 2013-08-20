#include <fruitlib/performance_timer.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
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

	std::chrono::milliseconds::rep const ms =
		std::chrono::duration_cast<std::chrono::milliseconds>(
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
