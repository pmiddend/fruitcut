#ifndef FRUITLIB_PP_FILTER_SSAA_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_SSAA_HPP_INCLUDED

#include <fruitlib/pp/filter/ivec2_parameter.hpp>
#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/filter/texture_size.hpp>
#include <fruitlib/pp/filter/unary.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/planar_texture.hpp>
#include <sge/shader/parameter/scalar.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class ssaa
:
	public fruitlib::pp::filter::unary
{
FCPPT_NONCOPYABLE(
	ssaa);
public:
	FCPPT_MAKE_STRONG_TYPEDEF(
		sge::renderer::scalar,
		scaling_factor);

	ssaa(
		fruitlib::pp::filter::manager &,
		fruitlib::pp::texture::manager &,
		fruitlib::pp::filter::texture_size const &);

	fruitlib::pp::texture::counted_instance const
	apply(
		fruitlib::pp::texture::counted_instance);

	~ssaa();
private:
	fruitlib::pp::filter::manager &filter_manager_;
	texture::manager &texture_manager_;
	fruitlib::pp::filter::texture_size const texture_size_;
	sge::shader::pair shader_;
	fruitlib::pp::filter::ivec2_parameter texture_size_parameter_;
	sge::shader::parameter::planar_texture texture_;
};
}
}
}

#endif
