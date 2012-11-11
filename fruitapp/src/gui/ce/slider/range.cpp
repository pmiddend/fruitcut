#include <fruitapp/gui/ce/slider/range.hpp>
#include <fcppt/assert/pre.hpp>

fruitapp::gui::ce::slider::range::range(
	fruitapp::gui::ce::slider::scalar const _min,
	fruitapp::gui::ce::slider::scalar const _max)
:
	min_(
		_min),
	max_(
		_max)
{
	FCPPT_ASSERT_PRE(
		_max > _min);
}

fruitapp::gui::ce::slider::scalar
fruitapp::gui::ce::slider::range::min() const
{
	return
		min_;
}

fruitapp::gui::ce::slider::scalar
fruitapp::gui::ce::slider::range::max() const
{
	return
		max_;
}
