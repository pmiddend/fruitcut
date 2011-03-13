#include "sound_group.hpp"
#include "../../create_rng.hpp"
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <boost/next_prior.hpp>
#include <iostream>

fruitcut::audio::detail::sound_group::sound_group(
	buffer_sequence const &_buffers)
:
	buffers_(
		_buffers),
	rng_(
		fcppt::random::make_last_exclusive_range<buffer_sequence::iterator::difference_type>(
			static_cast<buffer_sequence::iterator::difference_type>(
				0),
			static_cast<buffer_sequence::iterator::difference_type>(
				buffers_.size())),
		create_rng())
{
}

sge::audio::buffer_ptr const
fruitcut::audio::detail::sound_group::random_element()
{
	return 
		*boost::next(
			buffers_.begin(),
			rng_());
}
