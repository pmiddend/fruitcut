#ifndef FRUITLIB_RECTANGLE_MANAGER_RECTANGLE_INSTANCE_IMPL_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_MANAGER_RECTANGLE_INSTANCE_IMPL_HPP_INCLUDED

#include <fruitlib/rectangle_manager/object_impl.hpp>

template<typename T>
fruitlib::rectangle_manager::rectangle_instance<T>::rectangle_instance(
	manager_object &_manager,
	dim const &_bounds)
:
	manager_(
		_manager),
	bounds_(
		vector::null(),
		_bounds),
	status_fraction_(),
	killed_(
		false)
{
	manager_.insert(
		*this);
}

template<typename T>
typename
fruitlib::rectangle_manager::rectangle_instance<T>::rect const &
fruitlib::rectangle_manager::rectangle_instance<T>::bounds() const
{
	return bounds_;
}

template<typename T>
void
fruitlib::rectangle_manager::rectangle_instance<T>::pos(
	vector const &_pos)
{
	bounds_.pos(  
		_pos);
}

template<typename T>
void
fruitlib::rectangle_manager::rectangle_instance<T>::target(
	vector const &_target)
{
	target_ = 
		_target;
}

template<typename T>
typename
fruitlib::rectangle_manager::rectangle_instance<T>::vector const &
fruitlib::rectangle_manager::rectangle_instance<T>::target() const
{
	return 
		target_;
}

template<typename T>
void
fruitlib::rectangle_manager::rectangle_instance<T>::kill()
{
	killed_ = 
		true;
	manager_.kill(
		*this);
}

template<typename T>
typename
fruitlib::rectangle_manager::rectangle_instance<T>::value_type
fruitlib::rectangle_manager::rectangle_instance<T>::status_fraction() const
{
	return status_fraction_;
}

template<typename T>
void
fruitlib::rectangle_manager::rectangle_instance<T>::status_fraction(
	value_type const _status_fraction)
{
	status_fraction_ = 
		_status_fraction;
}

template<typename T>
bool
fruitlib::rectangle_manager::rectangle_instance<T>::killed() const
{
	return 
		killed_;
}

template<typename T>
fruitlib::rectangle_manager::rectangle_instance<T>::~rectangle_instance()
{
	manager_.erase(
		*this);
}

#endif
