#ifndef FRUITLIB_COUNTED_REFERENCE_DECL_HPP_INCLUDED
#define FRUITLIB_COUNTED_REFERENCE_DECL_HPP_INCLUDED

#include <fruitlib/counted_reference_fwd.hpp>
#include <fcppt/safe_bool.hpp>
#include <fcppt/function/object.hpp>

namespace fruitlib
{
template<typename T>
class counted_reference
{
// NO SEMICOLON HERE!
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
	fcppt::function::object<void (T&)>
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
}

#endif

