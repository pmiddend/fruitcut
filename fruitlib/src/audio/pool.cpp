#include <fruitlib/audio/pool.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/play_status.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitlib::audio::pool::pool()
:
	sounds_()
{
}

void
fruitlib::audio::pool::update()
{
	for(
		sound_sequence::iterator i =
			sounds_.begin();
		i != sounds_.end();)
	{
		i->update();
		if(i->status() == sge::audio::sound::play_status::stopped)
			i =
				sounds_.erase(
					i);
		else
			++i;
	}
}

void
fruitlib::audio::pool::insert(
	sge::audio::sound::base_unique_ptr _new)
{
	fcppt::container::ptr::push_back_unique_ptr(
		sounds_,
		std::move(
			_new));
}

fruitlib::audio::pool::~pool()
{
}
