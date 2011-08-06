#ifndef FRUITLIB_RECTANGLE_MANAGER_OBJECT_IMPL_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_MANAGER_OBJECT_IMPL_HPP_INCLUDED

#include <fruitlib/rectangle_manager/object_decl.hpp>
#include <fruitlib/exception.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/chrono/duration.hpp>
#include <boost/next_prior.hpp>

template<typename T>
fruitlib::rectangle_manager::object<T>::object(
	rect const &_bounding_rect,
	padding const &_padding,
	value_type const _speed)
:
	bounding_rect_(
		_bounding_rect),
	padding_(
		_padding),
	speed_(
		_speed),
	instances_(),
	dead_instances_()
{
}

template<typename T>
void
fruitlib::rectangle_manager::object<T>::update(
	duration const &d)
{
	value_type const epsilon = 
		0.01f;

	for(
		instance_iterator it = instances_.begin(); 
		it != instances_.end(); 
		++it)
	{
		it->pos(
			it->bounds().pos() + (it->target() - it->bounds().pos()) * d.count() * speed_);

		// To avoid jittering
		if(fcppt::math::vector::length(it->bounds().pos() - it->target()) < epsilon)
			it->pos(
				it->target());

		it->status_fraction(
			(it->bounds().pos().x() - it->target().x())/
			(it->bounds().size().w() + padding_.left()));
	}

	for(
		instance_iterator it = dead_instances_.begin(); 
		it != dead_instances_.end(); 
		++it)
	{
		it->pos(
			it->bounds().pos() + (it->target() - it->bounds().pos()) * d.count());

		// To avoid jittering
		if(fcppt::math::vector::length(it->bounds().pos() - it->target()) < epsilon)
			it->pos(
				it->target());

		it->status_fraction(
			1.0f +
			(it->target().x() - it->bounds().pos().x())/
			(it->bounds().size().w() + padding_.left()));
	}
}

template<typename T>
fruitlib::rectangle_manager::object<T>::~object()
{
}

template<typename T>
void
fruitlib::rectangle_manager::object<T>::insert(
	instance &_instance)
{
	_instance.status_fraction(
		-1.f);

	if(instances_.empty())
	{
		_instance.target(
			vector(
				padding_.left(),
				padding_.top()));

		_instance.pos(
			vector(
				bounding_rect_.left() - _instance.bounds().size().w(),
				_instance.target().y()));
	}
	else
	{
		rect const &last_rect = 
			instances_.back().bounds();

		_instance.target(
			vector(
				padding_.left(),
				last_rect.bottom() + padding_.middle()));

		_instance.pos(
			vector(
				bounding_rect_.left() - _instance.bounds().size().w(),
				_instance.target().y()));
	}
	
	instances_.push_back(
		&_instance);
}

template<typename T>
void
fruitlib::rectangle_manager::object<T>::kill(
	instance const &_instance)
{
	for(
		instance_iterator i = 
			instances_.begin(); 
		i != instances_.end(); 
		++i)
	{
		if(&(*i) != &_instance)
			continue;

		dead_instances_.push_back(
			&(*i));

		if(i != --instances_.end())
		{
			instance_iterator next_to_deleted = 
				boost::next(
					i);

			next_to_deleted->target(
				i->target());

			for(
				instance_iterator j = 
					boost::next(
						next_to_deleted); 
				j != instances_.end(); 
				++j)
			{
				instance &prior_j = 
					*boost::prior(
						j);

				j->target(
					vector(
						j->target().x(),
						prior_j.target().y() + prior_j.bounds().size().h() + padding_.middle()));
			}
		}

		// It's important that the target is set here, because the targets
		// for the following elements depend on i's old target.
		i->target(
			vector(
				bounding_rect_.left() - _instance.bounds().size().w(),
				_instance.bounds().pos().y()));

		instances_.erase(
			i);

		return;
	}

	throw 
		fruitlib::exception(
			FCPPT_TEXT("Tried to kill an instance which isn't found ")
			FCPPT_TEXT("in the live instance pool (maybe you've killed it twice?)"));
}

template<typename T>
void
fruitlib::rectangle_manager::object<T>::erase(
	instance const &_instance)
{
	if(_instance.killed())
		fcppt::algorithm::ptr_container_erase(
			dead_instances_,
			&_instance);
	else
		fcppt::algorithm::ptr_container_erase(
			instances_,
			&_instance);
}

#endif
