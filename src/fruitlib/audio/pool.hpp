#ifndef FRUITCUT_FRUITLIB_AUDIO_POOL_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_AUDIO_POOL_HPP_INCLUDED

#include <sge/audio/sound/base_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <vector>

namespace fruitcut
{
namespace fruitlib
{
namespace audio
{
class pool
{
FCPPT_NONCOPYABLE(
	pool);
public:
	explicit
	pool();

	void
	update();

	void
	insert(
		sge::audio::sound::base_ptr);

	~pool();
private:
	typedef
	std::vector<sge::audio::sound::base_ptr>
	sound_sequence;

	sound_sequence sounds_;
};
}
}
}

#endif
