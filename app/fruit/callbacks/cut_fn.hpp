#ifndef FRUITCUT_APP_FRUIT_CALLBACKS_CUT_FN_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_CALLBACKS_CUT_FN_HPP_INCLUDED

#include "../object_fwd.hpp"
#include "../area.hpp"

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace callbacks
{
// The scalar is the area
typedef
void
cut_fn(
	// old
	object const &,
	// new1
	object const &,
	// new2
	object const &,
	// cut area
	area);
}
}
}
}

#endif
