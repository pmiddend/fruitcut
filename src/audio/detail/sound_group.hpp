#ifndef FRUITCUT_AUDIO_DETAIL_SOUND_GROUP_HPP_INCLUDED
#define FRUITCUT_AUDIO_DETAIL_SOUND_GROUP_HPP_INCLUDED

#include "buffer_sequence.hpp"
#include "../../uniform_random.hpp"
#include <fcppt/random/uniform.hpp>

namespace fruitcut
{
namespace audio
{
namespace detail
{
class sound_group
{
public:
	explicit
	sound_group(
		buffer_sequence const &);

	// Cannot be const since rng is called
	sge::audio::buffer_ptr const
	random_element();
private:
	buffer_sequence buffers_;
	// Since we want to use it with boost::next, we take the difference type
	uniform_random<buffer_sequence::iterator::difference_type>::type rng_;
};
}
}
}

#endif
