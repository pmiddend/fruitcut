#ifndef FRUITLIB_COUNTED_REFERENCE_DECL_HPP_INCLUDED
#define FRUITLIB_COUNTED_REFERENCE_DECL_HPP_INCLUDED

#include <fruitlib/counted_reference_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
template<typename T>
class counted_reference
{
public:
	typedef
	T
	value_type;

	typedef
	value_type*
	pointer;

	typedef
	std::function<void (T&)>
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

	explicit
	operator
	bool() const;
private:
	typedef
	unsigned long
	size_type;

	pointer ptr_;
	size_type *count_;
	deleter deleter_;

	void
	dispose();
};
}

#endif

