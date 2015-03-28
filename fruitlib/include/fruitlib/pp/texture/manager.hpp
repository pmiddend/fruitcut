#ifndef FRUITLIB_PP_TEXTURE_MANAGER_HPP_INCLUDED
#define FRUITLIB_PP_TEXTURE_MANAGER_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/descriptor.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <sge/renderer/texture/planar_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <map>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace pp
{
namespace texture
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	FRUITLIB_DETAIL_SYMBOL
	explicit
	manager(
		sge::renderer::device::core &);

	// The texture descriptor may contain a dim thats equal to
	// "use_screen_size()". If that is the case, the texture is
	// considered a screen texture. Screen textures have to be deleted
	// as the viewport changes (because they have to be exactly the same
	// size as the viewport, else we wouldn't have a nice 1:1 pixel
	// mapping). Of course, this might delete a few non-screen textures,
	// too, but that neglegible.
	//
	// Alternatively, we could delete textures that aren't used for
	// "some time".
	fruitlib::pp::texture::counted_instance const
	query(
		descriptor const &);

	// This is used in situations where you want the output of one
	// filter chain to be the input of another filter chain without
	// losing the first result. You just hold a reference to the
	// original and fire up the second filter chain which will then not
	// use the locked texture.
	fruitlib::pp::texture::counted_instance const
	query(
		sge::renderer::texture::planar &);

	FRUITLIB_DETAIL_SYMBOL
	void
	clear_screen_textures();

	FRUITLIB_DETAIL_SYMBOL
	~manager();
private:
	typedef
	std::multimap
	<
		fruitlib::pp::texture::descriptor,
		std::unique_ptr
		<
			fruitlib::pp::texture::instance
		>
	>
	texture_map;

	sge::renderer::device::core &renderer_;
	texture_map textures_;

	fruitlib::pp::texture::counted_instance const
	query_internal(
		fruitlib::pp::texture::descriptor const &);
};
}
}
}

#endif
