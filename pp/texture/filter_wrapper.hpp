#ifndef FRUITCUT_PP_TEXTURE_FILTER_WRAPPER_HPP_INCLUDED
#define FRUITCUT_PP_TEXTURE_FILTER_WRAPPER_HPP_INCLUDED

#include <sge/renderer/filter/texture.hpp>

namespace fruitcut
{
namespace pp
{
namespace texture
{
class filter_wrapper
{
public:
	explicit
	filter_wrapper(
		sge::renderer::filter::texture const &);

	sge::renderer::filter::texture const &
	value() const;

	bool
	operator<(
		filter_wrapper const &) const;
private:
	sge::renderer::filter::texture value_;
};
}
}
}

#endif
