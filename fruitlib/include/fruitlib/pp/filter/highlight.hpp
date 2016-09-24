#ifndef FRUITLIB_PP_FILTER_HIGHLIGHT_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_HIGHLIGHT_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
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
class highlight
:
	public fruitlib::pp::filter::unary
{
FCPPT_NONCOPYABLE(
	highlight);
public:
	FCPPT_MAKE_STRONG_TYPEDEF(
		sge::renderer::scalar,
		threshold_factor);

	FRUITLIB_DETAIL_SYMBOL
	highlight(
		fruitlib::pp::filter::manager &,
		fruitlib::pp::texture::manager &,
		fruitlib::pp::filter::texture_size const &,
		threshold_factor const &);

	void
	threshold(
		threshold_factor const &);

	fruitlib::pp::texture::counted_instance const
	apply(
		fruitlib::pp::texture::counted_instance
	)
	override;

	FRUITLIB_DETAIL_SYMBOL
	~highlight()
	override;
private:
	fruitlib::pp::filter::manager &filter_manager_;
	texture::manager &texture_manager_;
	fruitlib::pp::filter::texture_size const texture_size_;
	sge::shader::pair shader_;
	sge::shader::parameter::scalar<threshold_factor::value_type> threshold_;
	sge::shader::parameter::planar_texture texture_;
};
}
}
}

#endif
