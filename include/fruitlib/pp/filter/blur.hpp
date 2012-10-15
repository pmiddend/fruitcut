#ifndef FRUITLIB_PP_FILTER_BLUR_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_BLUR_HPP_INCLUDED

#include <fruitlib/pp/filter/iterations.hpp>
#include <fruitlib/pp/filter/ivec2_parameter.hpp>
#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/filter/texture_size.hpp>
#include <fruitlib/pp/filter/unary.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/planar_texture.hpp>
#include <sge/shader/parameter/vector.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_array.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class blur
:
	public fruitlib::pp::filter::unary
{
FCPPT_NONCOPYABLE(
	blur);
public:
	blur(
		fruitlib::pp::filter::manager &,
		fruitlib::pp::texture::manager &,
		fruitlib::pp::filter::texture_size const &,
		fruitlib::pp::filter::iterations const &);

	fruitlib::pp::texture::counted_instance const
	apply(
		fruitlib::pp::texture::counted_instance);

	~blur();
private:
	FCPPT_MAKE_STRONG_TYPEDEF(
		std::size_t,
		instance_index);

	typedef
	fcppt::container::array<fruitlib::pp::texture::counted_instance,2>
	instance_array;

	typedef
	boost::ptr_array<sge::shader::pair,2u>
	shader_array;

	typedef
	boost::ptr_array<sge::shader::parameter::planar_texture,2u>
	planar_texture_array;

	typedef
	boost::ptr_array<fruitlib::pp::filter::ivec2_parameter,2u>
	texture_size_array;

	fruitlib::pp::filter::manager &filter_manager_;
	fruitlib::pp::texture::manager &texture_manager_;
	fruitlib::pp::filter::texture_size const texture_size_;
	fruitlib::pp::filter::iterations const iterations_;

	shader_array shaders_;
	planar_texture_array planar_textures_;
	texture_size_array texture_sizes_;

	void
	render(
		instance_array &,
		instance_index const &);
};
}
}
}

#endif
