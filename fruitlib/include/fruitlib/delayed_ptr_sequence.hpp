#ifndef FRUITLIB_DELAYED_PTR_SEQUENCE_HPP_INCLUDED
#define FRUITLIB_DELAYED_PTR_SEQUENCE_HPP_INCLUDED

#include <fcppt/unique_ptr.hpp>
#include <fcppt/algorithm/remove_if.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iterator>
#include <utility>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
template
<
	typename T,
	template<typename,typename> class PtrContainer
>
class delayed_ptr_sequence
{
public:
	typedef
	fcppt::unique_ptr<T>
	unique_value_ptr;

	// Those are just some of the types you'd have to define to get a
	// full blown container, but I was lazy.
	typedef
	PtrContainer<
		unique_value_ptr,
		std::allocator<T>
	>
	implementation_sequence;

	typedef typename
	implementation_sequence::iterator
	iterator;

	typedef typename
	implementation_sequence::const_iterator
	const_iterator;

	typedef typename
	implementation_sequence::size_type
	size_type;

	typedef typename
	implementation_sequence::reference
	reference;

	typedef typename
	implementation_sequence::const_reference
	const_reference;

	delayed_ptr_sequence()
	:
		implementation_(),
		new_values_(),
		old_values_()
	{
	}

	size_type
	size() const
	{
		return implementation_.size();
	}

	bool
	empty() const
	{
		return implementation_.empty();
	}

	reference
	front()
	{
		return implementation_.front();
	}

	const_reference
	front() const
	{
		return implementation_.front();
	}

	iterator const
	begin()
	{
		return implementation_.begin();
	}

	iterator const
	end()
	{
		return implementation_.end();
	}

	const_iterator const
	begin() const
	{
		return implementation_.begin();
	}

	const_iterator const
	end() const
	{
		return implementation_.end();
	}

	const_iterator const
	cbegin() const
	{
		return implementation_.begin();
	}

	const_iterator const
	cend() const
	{
		return implementation_.end();
	}

	void
	push_back(
		unique_value_ptr &&new_value)
	{
		new_values_.push_back(
			std::move(
				new_value));
	}

	// This is purely a helper function for me in a special case: I want
	// to transfer a separate "implementation_sequence" to this sequence
	// but without writing a getter for our implementation_
	void
	transfer_from(
		implementation_sequence &other_implementation)
	{
		new_values_.insert(
			new_values_.end(),
			std::make_move_iterator(
				other_implementation.begin()),
			std::make_move_iterator(
				other_implementation.end()));

		other_implementation.clear();
	}

	void
	erase(
		T const &t)
	{
		old_values_.push_back(
			&const_cast<T &>(
				t));
	}

	void
	update()
	{
		implementation_.insert(
			implementation_.end(),
			std::make_move_iterator(
				new_values_.begin()),
			std::make_move_iterator(
				new_values_.end()));

		new_values_.clear();

		FCPPT_ASSERT_ERROR(
			new_values_.empty());

		for(
			T *old
			:
			old_values_
		)
			fcppt::algorithm::remove_if(
				implementation_,
				[
					old
				](
					unique_value_ptr const &_ptr
				)
				{
					return
						_ptr.get_pointer()
						==
						old;
				}
			);

		old_values_.clear();
	}
private:
	typedef
	std::vector<
		T *
	>
	view_implementation_sequence;

	implementation_sequence implementation_;
	implementation_sequence new_values_;
	view_implementation_sequence old_values_;
};
}

#endif
