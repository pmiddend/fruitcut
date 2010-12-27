#include "system.hpp"
#include <sge/sprite/default_equal.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/move.hpp>

fruitcut::particle::system::system(
	sge::renderer::device_ptr const _renderer)
:
	pss_(
		_renderer),
	ss_(
		_renderer)
{
}

void
fruitcut::particle::system::update()
{
	for(particle_sequence::iterator i = particles_.begin(); i != particles_.end();)
	{
		i->update();
		if (i->dead())
			i = 
				particles_.erase(
					i);
		else
			++i;
	}
}

void 
fruitcut::particle::system::render()
{
	ss_.render_all(
		sge::sprite::default_equal());
}

fruitcut::sprite::system &
fruitcut::particle::system::sprite_system()
{
	return ss_;
}

fruitcut::particle::point_sprite::system &
fruitcut::particle::system::point_sprite_system()
{
	return pss_;
}

void 
fruitcut::particle::system::insert(
	objects::unique_base_ptr o)
{
	fcppt::container::ptr::push_back_unique_ptr(
		particles_,
		fcppt::move(
			o));
}

fruitcut::particle::system::~system()
{
}
