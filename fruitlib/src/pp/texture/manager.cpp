#include <fruitlib/pp/texture/counted_instance.hpp>
#include <fruitlib/pp/texture/instance.hpp>
#include <fruitlib/pp/texture/manager.hpp>
#include <fruitlib/pp/texture/use_screen_size.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/depth_stencil_format.hpp>
#include <sge/renderer/depth_stencil_surface.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/device/core.hpp>
#include <sge/renderer/target/base.hpp>
#include <sge/renderer/target/from_texture.hpp>
#include <sge/renderer/target/offscreen.hpp>
#include <sge/renderer/target/onscreen.hpp>
#include <sge/renderer/texture/capabilities.hpp>
#include <sge/renderer/texture/capabilities_field.hpp>
#include <sge/renderer/texture/depth_stencil.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/planar_parameters.hpp>
#include <sge/renderer/texture/planar_unique_ptr.hpp>
#include <sge/renderer/texture/mipmap/off.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/optional_impl.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/unreachable_message.hpp>
#include <fcppt/container/ptr/insert_unique_ptr_multimap.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/comparison.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitlib::pp::texture::manager::manager(
	sge::renderer::device::core &_renderer)
:
	renderer_(
		_renderer),
	textures_()
{
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::texture::manager::query(
	fruitlib::pp::texture::descriptor const &d)
{
	return
		this->query_internal(
			fruitlib::pp::texture::descriptor(
				d.size() == fruitlib::pp::texture::use_screen_size()
				?
					fcppt::math::dim::structure_cast<sge::renderer::dim2>(
						renderer_.onscreen_target().viewport().get().size())
				:
					d.size(),
				d.image_format(),
				d.depth_stencil()));
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::texture::manager::query(
	sge::renderer::texture::planar &t)
{
	for (texture_map::iterator i = textures_.begin(); i != textures_.end(); ++i)
	{
		if (i->second->texture().get() == &t)
		{
			i->second->locked(
				true);
			return
				counted_instance(
					*(i->second),
					std::tr1::bind(
						static_cast<void (instance::*)(bool)>(
							&instance::locked),
						std::tr1::placeholders::_1,
						false));
		}
	}

	FCPPT_ASSERT_UNREACHABLE_MESSAGE(
		FCPPT_TEXT("Tried to lock a texture which isn't there!"));
}

void
fruitlib::pp::texture::manager::clear_screen_textures()
{
	sge::renderer::dim2 const onscreen_dim =
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			renderer_.onscreen_target().viewport().get().size());

	// From the standard about associative containers:
	// "and the erase members shall invalidate only iterators and references to the erased elements."
	for(
		texture_map::iterator
			it =
				textures_.begin(),
			next =
				it;
		it != textures_.end();
		it = next)
	{
		++next;
		if(it->first.size() == onscreen_dim && !it->second->locked())
			textures_.erase(
				it);
	}
}

fruitlib::pp::texture::manager::~manager()
{
}

fruitlib::pp::texture::counted_instance const
fruitlib::pp::texture::manager::query_internal(
	descriptor const &d)
{
	boost::iterator_range<texture_map::iterator> eq_range =
		textures_.equal_range(
			d);

	for(
		texture_map::iterator i = eq_range.begin();
		i != eq_range.end();
		++i)
	{
		// Texture is correct, but it's locked
		if (i->second->locked())
			continue;

		// Texture is correct and not locked!
		i->second->locked(
			true);
		return
			counted_instance(
				*(i->second),
				std::tr1::bind(
					static_cast<void (instance::*)(bool)>(
						&instance::locked),
					std::tr1::placeholders::_1,
					false));
	}

	sge::renderer::texture::planar_unique_ptr new_texture =
		renderer_.create_planar_texture(
			sge::renderer::texture::planar_parameters(
				d.size(),
				sge::renderer::texture::color_format(
					d.image_format(),
					sge::renderer::texture::emulate_srgb::no),
				sge::renderer::texture::mipmap::off(),
				sge::renderer::resource_flags_field::null(),
				sge::renderer::texture::capabilities_field(
					sge::renderer::texture::capabilities::render_target)));

	sge::renderer::target::offscreen_unique_ptr new_target(
		sge::renderer::target::from_texture(
			renderer_,
			*new_texture));

	sge::renderer::depth_stencil_surface_unique_ptr new_target_depth_stencil;

	switch (d.depth_stencil())
	{
		case depth_stencil_format::off:
			break;
		case depth_stencil_format::d16:
			new_target_depth_stencil =
				renderer_.create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d16);
			break;
		case depth_stencil_format::d32:
			new_target_depth_stencil =
				renderer_.create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d32);
			break;
		case depth_stencil_format::d24s8:
			new_target_depth_stencil =
				renderer_.create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d24s8);
			break;
	}

	if(new_target_depth_stencil)
		new_target->depth_stencil_surface(
			sge::renderer::optional_depth_stencil_surface_ref(
				*new_target_depth_stencil));

	// There are no matching textures? Gotta create a new one!
	texture_map::iterator const result =
		fcppt::container::ptr::insert_unique_ptr_multimap(
			textures_,
			d,
			fcppt::make_unique_ptr<fruitlib::pp::texture::instance>(
				d,
				fcppt::move(
					new_texture),
				fcppt::move(
					new_target),
				// FIXME: Can we move a null unique_ptr?
				fcppt::move(
					new_target_depth_stencil),
				fruitlib::pp::texture::is_locked(
					true)));
	return
		fruitlib::pp::texture::counted_instance(
			*result->second,
			std::tr1::bind(
				static_cast<void (instance::*)(bool)>(
					&instance::locked),
				std::tr1::placeholders::_1,
				false));
}
