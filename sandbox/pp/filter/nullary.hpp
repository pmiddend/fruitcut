#ifndef FRUITCUT_SANDBOX_PP_FILTER_NULLARY_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_FILTER_NULLARY_HPP_INCLUDED

#include "base.hpp"
#include <sge/renderer/texture_ptr.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
namespace filter
{
class nullary
:
	public base
{
public:
	virtual sge::renderer::texture_ptr const
	apply() = 0;
};
}
}
}
}

#endif
