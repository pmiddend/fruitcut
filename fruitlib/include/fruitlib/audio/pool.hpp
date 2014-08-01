#ifndef FRUITLIB_AUDIO_POOL_HPP_INCLUDED
#define FRUITLIB_AUDIO_POOL_HPP_INCLUDED

#include <sge/audio/sound/base_unique_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


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
		sge::audio::sound::base_unique_ptr &&);

	~pool();
private:
	typedef
	std::vector
	<
		sge::audio::sound::base_unique_ptr
	>
	sound_sequence;

	sound_sequence sounds_;
};
}
}

#endif
