#ifndef FRUITLIB_COUNTED_REFERENCE_IMPL_HPP_INCLUDED
#define FRUITLIB_COUNTED_REFERENCE_IMPL_HPP_INCLUDED

#include <fruitlib/counted_reference_decl.hpp>

template<typename T>
fruitlib::counted_reference<T>::counted_reference()
:
	ptr_(
		nullptr),
	count_(
		new size_type(
			1)),
	deleter_()
{
}

template<typename T>
fruitlib::counted_reference<T>::counted_reference(
	T &_value,
	deleter const &_deleter)
:
	ptr_(
		&_value),
	count_(
		new size_type(
			1)),
	deleter_(
		_deleter)
{
}

template<typename T>
fruitlib::counted_reference<T>::counted_reference(
	counted_reference const &other)
:
	ptr_(
		other.ptr_),
	count_(
		other.count_),
	deleter_(
		other.deleter_)
{
	++(*count_);
}

template<typename T>
fruitlib::counted_reference<T>::~counted_reference()
{
	dispose();
}

template<typename T>
fruitlib::counted_reference<T> &
fruitlib::counted_reference<T>::operator=(
	counted_reference const &other)
{
	if (this != &other)
	{
		dispose();
		ptr_ = other.ptr_;
		count_ = other.count_;
		deleter_ = other.deleter_;
		++(*count_);
	}
	return *this;
}

template<typename T>
typename
fruitlib::counted_reference<T>::pointer
fruitlib::counted_reference<T>::operator->() const
{
	return ptr_;
}

template<typename T>
fruitlib::counted_reference<T>::operator bool() const
{
	return ptr_ != nullptr;
}

template<typename T>
void
fruitlib::counted_reference<T>::dispose()
{
	if (--(*count_) == 0u)
	{
		if (deleter_ && ptr_)
		{
			deleter_(
				*ptr_);
		}
		delete count_;
	}
}

#endif

