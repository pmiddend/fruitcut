#include "pool.hpp"
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/play_status.hpp>

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
		(*i)->update();
		if((*i)->status() == sge::audio::sound::play_status::stopped)
			i = 
				sounds_.erase(
					i);
		else
			++i;
	}
}

void
fruitlib::audio::pool::insert(
	sge::audio::sound::base_ptr const _new)
{
	sounds_.push_back(
		_new);
}

fruitlib::audio::pool::~pool()
{
}
