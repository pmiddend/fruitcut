#ifndef FRUITLIB_PP_FILTER_RENDER_TO_TEXTURE_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_RENDER_TO_TEXTURE_HPP_INCLUDED

#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/filter/nullary.hpp>
#include <fruitlib/pp/filter/texture_size.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/depth_stencil_format.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/clear/parameters.hpp>
#include <sge/renderer/context/core_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/math/dim/object_impl.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class render_to_texture
:
	public nullary
{
FCPPT_NONCOPYABLE(
	render_to_texture);
public:
	typedef
	fcppt::function::object<void (sge::renderer::context::core &)>
	callback;

	render_to_texture(
		fruitlib::pp::filter::manager &,
		fruitlib::pp::texture::manager &,
		fruitlib::pp::filter::texture_size const &,
		sge::renderer::clear::parameters const &,
		callback const &,
		fruitlib::pp::texture::depth_stencil_format::type);

	fruitlib::pp::texture::counted_instance const
	apply();

	~render_to_texture();
private:
	fruitlib::pp::filter::manager &filter_manager_;
	texture::manager &texture_manager_;
	fruitlib::pp::filter::texture_size const texture_size_;
	sge::renderer::clear::parameters const clear_parameters_;
	callback const callback_;
	fruitlib::pp::texture::depth_stencil_format::type const depth_stencil_;
};
}
}
}

#endif
