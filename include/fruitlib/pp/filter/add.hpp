#ifndef FRUITLIB_PP_FILTER_ADD_HPP_INCLUDED
#define FRUITLIB_PP_FILTER_ADD_HPP_INCLUDED

#include <fruitlib/pp/filter/binary.hpp>
#include <fruitlib/pp/filter/manager_fwd.hpp>
#include <fruitlib/pp/filter/texture_size.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/manager_fwd.hpp>
#include <sge/shader/pair.hpp>
#include <sge/shader/parameter/planar_texture.hpp>
#include <sge/shader/parameter/vector.hpp>
#include <fcppt/noncopyable.hpp>


namespace fruitlib
{
namespace pp
{
namespace filter
{
class add
:
	public fruitlib::pp::filter::binary
{
FCPPT_NONCOPYABLE(
	add);
public:
	add(
		fruitlib::pp::filter::manager &,
		fruitlib::pp::texture::manager &,
		fruitlib::pp::filter::texture_size const &);

	fruitlib::pp::texture::counted_instance const
	apply(
		fruitlib::pp::texture::counted_instance,
		fruitlib::pp::texture::counted_instance);

	~add();
private:
	fruitlib::pp::filter::manager &filter_manager_;
	fruitlib::pp::texture::manager &texture_manager_;
	fruitlib::pp::filter::texture_size const dimension_;
	sge::shader::pair shader_;
	sge::shader::parameter::planar_texture texture1_;
	sge::shader::parameter::planar_texture texture2_;
};
}
}
}

#endif
