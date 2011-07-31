#ifndef FRUITLIB_RECTANGLE_ALIGNER_HPP_INCLUDED
#define FRUITLIB_RECTANGLE_ALIGNER_HPP_INCLUDED

#include <fcppt/noncopyable.hpp>
#include <fcppt/math/box/box.hpp>
#include <list>

namespace fruitlib
{
template<typename T>
class rectangle_aligner
{
FCPPT_NONCOPYABLE(
	rectangle_aligner);
public:
	typedef
	T
	value_type;

	typedef typename
	fcppt::math::box::rect<T>::type
	rect;

	explicit
	rectangle_aligner(
		rect const &_bounding_rect)
	:
		bounding_rect_(
			_bounding_rect)
	{
	}
private:
	typedef
	std::list<rect>
	rect_sequence;

	rect const bounding_rect_;
	rect_sequence rects_;
};
}

#endif
