#include "system.hpp"
#include "cache.hpp"
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <iostream>

fruitcut::font::system::system(
	cache &_cache)
:
	cache_(
		_cache)
{
}

void
fruitcut::font::system::update()
{
	for(
		intrusive_list::iterator i = 
			particles_.begin(); 
		i != particles_.end(); 
		++i)
		i->update();
}

void
fruitcut::font::system::render()
{
	for(
		intrusive_list::iterator i = 
			particles_.begin(); 
		i != particles_.end(); 
		++i)
	{
		i->render(
			cache_.metrics(
				i->metrics()),
			cache_.drawer(
				i->metrics()));
	}
}

fruitcut::font::system::~system()
{
}

void
fruitcut::font::system::insert(
	particle::base &o)
{
	particles_.push_back(
		o);
}

