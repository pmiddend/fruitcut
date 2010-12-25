#ifndef FRUITCUT_PP_FILTER_RENDER_TO_TEXTURE_HPP_INCLUDED
#define FRUITCUT_PP_FILTER_RENDER_TO_TEXTURE_HPP_INCLUDED

#include "nullary.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/dim2.hpp>
#include <boost/function.hpp>

namespace fruitcut
{
namespace pp
{
namespace filter
{
class render_to_texture
:
	public nullary
{
public:
	typedef
	boost::function<void ()>
	callback;

	explicit	
	render_to_texture(
		sge::renderer::device_ptr,
		sge::renderer::dim2 const &,
		callback const &);

	sge::renderer::texture_ptr const
	apply();

	~render_to_texture();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::texture_ptr const texture_;
	sge::renderer::target_ptr const target_;
	callback const callback_;
};
}
}
}

#endif
