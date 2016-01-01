#include <fruitlib/performance_timer.hpp>
#include <fcppt/const.hpp>
#include <fcppt/optional/maybe.hpp>
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

	if(
		fcppt::optional::maybe(
			threshold_,
			fcppt::const_(
				true
			),
			[
				diff
			](
				clock::duration const _threshold
			)
			{
				return
					diff > _threshold;
			}
		)
	)
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
