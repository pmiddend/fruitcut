#ifndef FRUITAPP_GUI_CE_SLIDER_RANGE_HPP_INCLUDED
#define FRUITAPP_GUI_CE_SLIDER_RANGE_HPP_INCLUDED

#include <fruitapp/gui/ce/slider/scalar.hpp>

namespace fruitapp
{
namespace gui
{
namespace ce
{
namespace slider
{
class range
{
public:
	range(
		fruitapp::gui::ce::slider::scalar min,
		fruitapp::gui::ce::slider::scalar max);

	fruitapp::gui::ce::slider::scalar
	min() const;

	fruitapp::gui::ce::slider::scalar
	max() const;
private:
	fruitapp::gui::ce::slider::scalar min_;
	fruitapp::gui::ce::slider::scalar max_;
};
}
}
}
}

#endif
