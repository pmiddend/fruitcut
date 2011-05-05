#ifndef FRUITCUT_PAIR_ITERATOR_HPP_INCLUDED
#define FRUITCUT_PAIR_ITERATOR_HPP_INCLUDED

// NOTE: This isn't used anywhere, it's just an attempt to simplify
// cut_mesh.cpp
namespace fruitcut
{
template<typename Iterator>
class pair_iterator
:
	public 
		boost::iterator_facade
		<
			pair_iterator<Iterator>,
			std::pair<Iterator,Iterator>,
			// NOTE: Where does this come from (which header)?
			boost::forward_traversal_tag
		>
{
public:
	typedef
	Iterator
	iterator_impl;

	typedef
	std::pair<iterator_impl,iterator_impl>
	value_type;

	// We need this to satisfy the forward traversal iterator
	// requirements
	explicit
	pair_iterator()
	:
		its_(),
		end_()
	{
	}

	explicit
	pair_iterator(
		iterator_impl const &it,
		iterator_impl const &_end)
	:
		its_(
			value_type(
				it,
				boost::next(
					it))),
		end_(
			_end)
	{
	}

	bool
	at_end() const
	{
		return its_.first == end_;
	}
private:
	value_type its_;
	iterator_impl end_;

	friend class boost::iterator_core_access;

	void 
	increment() 
	{ 
		if (its_.second == boost::prior(end_) && its_.first != end_)
		{
			its_.first++;
			its_.second = 
				boost::next(
					its_.first);
		}
		else
			its_.second++;
	}

	bool 
	equal(
		pair_iterator const& other) const
	{
		return its_ == other.its_ && end_ == other.end_;
	}

	value_type &
	dereference() const 
	{ 
		// FIXME
		return const_cast<pair_iterator &>(*this).its_;
	}
};
}

#endif
