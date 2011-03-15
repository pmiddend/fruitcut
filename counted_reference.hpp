#ifndef FRUITCUT_COUNTED_REFERENCE_HPP_INCLUDED
#define FRUITCUT_COUNTED_REFERENCE_HPP_INCLUDED

#include <fcppt/safe_bool.hpp>
#include <fcppt/tr1/functional.hpp>

namespace fruitcut
{
template<typename T>
class counted_reference 
{
// NO COMMA HERE!
FCPPT_SAFE_BOOL(
	counted_reference)
public:
	typedef
	T
	value_type;

	typedef
	value_type*
	pointer;

	typedef
	std::tr1::function<void (T&)>
	deleter;

	explicit 
	counted_reference();

	explicit 
	counted_reference( 
		T &,
		deleter const & = deleter());

	counted_reference(
		counted_reference const &);

	~counted_reference();

	counted_reference &
	operator=(
		counted_reference const &);

	pointer 
	operator->() const;
private:
	typedef
	unsigned long
	size_type;

	pointer ptr_;
	size_type *count_;
	deleter deleter_;

	void
	dispose();

	bool
	boolean_test() const;
};

template<typename T>
counted_reference<T>::counted_reference()
:
	ptr_(
		0),
	count_(
		new size_type(
			1))
{
}

template<typename T>
counted_reference<T>::counted_reference(
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
counted_reference<T>::counted_reference(
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
counted_reference<T>::~counted_reference()
{
	dispose();
}

template<typename T>
counted_reference<T> &
counted_reference<T>::operator=(
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
counted_reference<T>::pointer 
counted_reference<T>::operator->() const
{
	return ptr_;
}

template<typename T>
void
counted_reference<T>::dispose()
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

template<typename T>
bool
counted_reference<T>::boolean_test() const
{
	return ptr_;
}
}

#endif
