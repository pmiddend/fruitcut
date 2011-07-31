#ifndef FRUITLIB_RECTANGLE_MANAGER_PADDING_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_MANAGER_PADDING_HPP_INCLUDED

namespace fruitlib
{
namespace rectangle_manager
{
template<typename T>
class padding
{
private:
	T const left_,top_,middle_;
public:
	explicit
	padding(
		T const _left,
		T const _top,
		T const _middle)
	:
		left_(
			_left),
		top_(
			_top),
		middle_(
			_middle)
	{
	}

	T 
	left() const
	{
		return left_;
	}

	T 
	top() const
	{
		return top_;
	}

	T 
	middle() const
	{
		return middle_;
	}
};
}
}

#endif
