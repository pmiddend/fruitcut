#ifndef FRUITLIB_RECTANGLE_MANAGER_OBJECT_IMPL_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_MANAGER_OBJECT_IMPL_HPP_INCLUDED

#include <fruitlib/rectangle_manager/object_decl.hpp>
#include <fruitlib/exception.hpp>
#include <fruitlib/math/line/point_intersection.hpp>
#include <fruitlib/math/line/basic.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/chrono/duration.hpp>
#include <boost/next_prior.hpp>

template<typename T>
fruitlib::rectangle_manager::object<T>::object(
	rect const &_bounding_rect,
	padding const &_padding,
	value_type const _acceleration)
:
	bounding_rect_(
		_bounding_rect),
	padding_(
		_padding),
	acceleration_(
		_acceleration),
	instances_(),
	dead_instances_()
{
}

template<typename T>
void
fruitlib::rectangle_manager::object<T>::update(
	duration const &d)
{
	for(
		instance_iterator it = instances_.begin(); 
		it != instances_.end(); 
		++it)
	{
		this->update_kinematics(
			d,
			*it);

		it->status_fraction(
			(it->bounds().pos().x() - it->target().x())/
			(it->bounds().size().w() + padding_.left()));
	}

	for(
		instance_iterator it = dead_instances_.begin(); 
		it != dead_instances_.end(); 
		++it)
	{
		this->update_kinematics(
			d,
			*it);

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
	this->insert_impl(
		_instance,
		true);
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
fruitlib::rectangle_manager::object<T>::revive(
	instance &_instance)
{
	FCPPT_ASSERT(
		_instance.killed());
	
	fcppt::algorithm::ptr_container_erase(
		dead_instances_,
		&_instance);

	this->insert_impl(
		_instance,
		false);
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

template<typename T>
void
fruitlib::rectangle_manager::object<T>::update_kinematics(
	duration const &d,
	instance &_instance)
{
	vector const pos_to_target_vector = 
		_instance.target() - _instance.bounds().pos();

	value_type const distance_to_target = 
		fcppt::math::vector::length(
			pos_to_target_vector);

	// Bail out if nothing has to be done
	if(distance_to_target < 0.01f)
	{
		_instance.pos(
			_instance.target());
		_instance.speed(
			0.0f);
		return;
	}

	value_type const speed_magnitude = 
		d.count() * 
		_instance.speed();

	if(speed_magnitude < 0.001f)
	{
		_instance.speed(
			_instance.speed() + d.count() * acceleration_);
		return;
	}

	vector const speed_vector = 
		speed_magnitude *
		// This call is safe now, since we have the if above
		pos_to_target_vector / distance_to_target;

	// Check if we hit the target along our way, using the current speed.
	value_type const target_intersection = 
		fruitlib::math::line::point_intersection(
			_instance.target(),
			// Constructing a line is safe now, because speed_vector's
			// size is large enough
			fruitlib::math::line::basic<value_type,2>(
				_instance.bounds().pos(),
				speed_vector));

	// We got a hit!
	if(target_intersection < 1.0f)
	{
		_instance.pos(
			_instance.target());
		_instance.speed(
			0.0f);
	}
	else
	{
		_instance.pos(
			_instance.bounds().pos() + speed_vector);
		_instance.speed(
			// We want to add acceleration to the speed, but we cannot just
			// say "speed + acceleration" since that'll just add the number
			// "acceleration" component-wise. What we really want to do is
			// add to the tip of the speed vector.
			_instance.speed() + d.count() * acceleration_);
	}
}

template<typename T>
void
fruitlib::rectangle_manager::object<T>::insert_impl(
	instance &_instance,
	bool const set_position)
{
	vector position;
	vector target;

	if(instances_.empty())
	{
		target = 
			vector(
				padding_.left(),
				padding_.top());

		position = 
			vector(
				bounding_rect_.left() - _instance.bounds().size().w(),
				target.y());
	}
	else
	{
		rect const &last_rect = 
			instances_.back().bounds();

		target = 
			vector(
				padding_.left(),
				last_rect.bottom() + padding_.middle());

		position = 
			vector(
				bounding_rect_.left() - _instance.bounds().size().w(),
				target.y());
	}

	_instance.target(
		target);

	if(set_position)
		_instance.pos(
			position);
	
	instances_.push_back(
		&_instance);
}

#endif
