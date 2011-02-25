#ifndef FRUITCUT_DELAYED_PTR_SEQUENCE_HPP_INCLUDED
#define FRUITCUT_DELAYED_PTR_SEQUENCE_HPP_INCLUDED

#include <fcppt/unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <boost/ptr_container/clone_allocator.hpp>
#include <memory>

namespace fruitcut
{
template
<
	typename T,
	template<typename,typename,typename> class PtrContainer,
	typename CloneAllocator = boost::heap_clone_allocator,
	typename Allocator = std::allocator<void *>
>
class delayed_ptr_sequence
{
public:
	// Those are just some of the types you'd have to define to get a
	// full blown container, but I was lazy.
	typedef
	PtrContainer<T,CloneAllocator,Allocator>
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

	typedef
	fcppt::unique_ptr<T>
	unique_value_ptr;

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
		unique_value_ptr new_value)
	{
		fcppt::container::ptr::push_back_unique_ptr(
			new_values_,
			fcppt::move(
				new_value));
	}

	void
	erase(
		T &t)
	{
		old_values_.push_back(
			&t);
	}

	void
	update()
	{
		implementation_.transfer(
			implementation_.end(),
			new_values_.begin(),
			new_values_.end(),
			new_values_);

		for(iterator i = old_values_.begin(); i != old_values_.end(); ++i)
			fcppt::algorithm::ptr_container_erase(
				implementation_,
				&(*i));
	}
private:
	// Also because of lazyness, I used the same ptr_sequence here, but
	// it could equally well be a vector<T*> or something.
	typedef
	PtrContainer
	<
		T,
		boost::view_clone_allocator,
		Allocator
	>
	view_implementation_sequence;

	implementation_sequence implementation_;
	implementation_sequence new_values_;
	view_implementation_sequence old_values_;
};
}

#endif
