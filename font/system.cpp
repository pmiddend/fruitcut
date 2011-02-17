#include "system.hpp"
#include "particle/unique_base_ptr.hpp"
#include <sge/image/colors.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <boost/spirit/home/phoenix/core.hpp>

fruitcut::font::system::system(
	sge::renderer::device_ptr const _renderer)
:
	drawer_(
		_renderer,
		sge::image::colors::white(),
		boost::phoenix::arg_names::arg2)
{
}

void
fruitcut::font::system::insert(
	particle::unique_base_ptr o)
{
	fcppt::container::ptr::push_back_unique_ptr(
		nonintrusive_particles_,
		fcppt::move(
			o));
}

void
fruitcut::font::system::insert(
	particle::base &o)
{
	intrusive_particles_.push_back(
		o);
}

void
fruitcut::font::system::update()
{
	for(
		nonintrusive_list::iterator i = 
			nonintrusive_particles_.begin(); 
		i != nonintrusive_particles_.end(); )
	{
		i->update();
		if (i->dead())
			i = 
				nonintrusive_particles_.erase(
					i);
		else
			++i;
	}

	for(
		intrusive_list::iterator i = 
			intrusive_particles_.begin(); 
		i != intrusive_particles_.end(); 
		++i)
		i->update();
}

void
fruitcut::font::system::render()
{
	for(
		nonintrusive_list::iterator i = 
			nonintrusive_particles_.begin(); 
		i != nonintrusive_particles_.end();
		++i)
		i->render(
			drawer_);

	for(
		intrusive_list::iterator i = 
			intrusive_particles_.begin(); 
		i != intrusive_particles_.end(); 
		++i)
		i->render(
			drawer_);
}
