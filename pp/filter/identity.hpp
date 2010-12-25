#ifndef FRUITCUT_PP_FILTER_IDENTITY_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_IDENTITY_HPP_INCLUDED

#include "unary.hpp"
#include <sge/renderer/texture_ptr.hpp>
#include <boost/function.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class identity
:
	public unary
{
public:
	typedef
	boost::function<void ()>
	callback;

	explicit
	identity(
		callback const &);

	sge::renderer::texture_ptr const
	apply(
		sge::renderer::texture_ptr);
private:
	callback const callback_;
};
}
}
}

#endif
