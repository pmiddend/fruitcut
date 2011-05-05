#ifndef FRUITCUT_PP_TEXTURE_FILTER_WRAPPER_HPP_INCLUDED
#define FRUITCUT_PP_TEXTURE_FILTER_WRAPPER_HPP_INCLUDED

#include <sge/renderer/texture/filter/object.hpp>

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
		sge::renderer::texture::filter::object const &);

	sge::renderer::texture::filter::object const &
	value() const;

	bool
	operator<(
		filter_wrapper const &) const;
private:
	sge::renderer::texture::filter::object value_;
};
}
}
}

#endif
