#ifndef FRUITCUT_PP_FILTER_BINARY_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_BINARY_HPP_INCLUDED

#include "base.hpp"
#include <sge/renderer/texture_ptr.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class binary
:
	public base
{
public:
	virtual sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr,
		sge::renderer::texture_ptr) = 0;
};
}
}
}

#endif
