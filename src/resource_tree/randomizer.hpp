#ifndef FRUITCUT_RESOURCE_TREE_RANDOMIZER_HPP_INCLUDED
#define FRUITCUT_RESOURCE_TREE_RANDOMIZER_HPP_INCLUDED

#include "make_type.hpp"
#include "leaf_value.hpp"
#include "../random_generator.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/random/uniform.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <boost/next_prior.hpp>
#include <iterator>

namespace fruitcut
{
namespace resource_tree
{
template<typename T>
class randomizer
{
FCPPT_NONCOPYABLE(
	randomizer);
public:
	typedef typename
	resource_tree::make_type<T>::type
	tree_type;

	typedef typename 
	std::iterator_traits<typename tree_type::child_list::iterator>::difference_type
	difference_type;

	explicit
	randomizer(
		tree_type const &_tree,
		random_generator const &generator)
	:
		tree_(
			_tree),
		rng_(
			fcppt::random::make_last_exclusive_range(
				static_cast<difference_type>(
					0),
				static_cast<difference_type>(
					tree_.size())),
			generator)
	{
	}

	T const &
	choose_random()

	{
		return 
			resource_tree::leaf_value<T>(
				*boost::next(
					tree_.begin(),
					rng_()));
	}

	~randomizer()
	{
	}
private:
	typedef
	fcppt::random::uniform<difference_type,random_generator>
	rng;

	tree_type const &tree_;
	rng rng_;
};
}
}

#endif
