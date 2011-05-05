#include "manager.hpp"
#include "instance.hpp"
#include "use_screen_size.hpp"
#include "counted_instance.hpp"
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/target_from_texture.hpp>
#include <sge/renderer/depth_stencil_format.hpp>
#include <sge/renderer/target.hpp>
#include <sge/renderer/texture/planar_parameters.hpp>
#include <sge/renderer/texture/capabilities_field.hpp>
#include <sge/renderer/texture/capabilities.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/image/color/format.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/comparison.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/text.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <utility>
#include <memory>

fruitcut::pp::texture::manager::manager(
	sge::renderer::device &_renderer)
:
	renderer_(
		_renderer),
	textures_()
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query(
	descriptor const &d)
{
	return 
		query_internal(
			descriptor(
				d.size() == use_screen_size()
				?
					fcppt::math::dim::structure_cast<sge::renderer::dim2>(
						renderer_.onscreen_target().viewport().get().size())
				:
					d.size(),
				d.image_format(),
				d.filter(),
				d.depth_stencil()));
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query(
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

	FCPPT_ASSERT_MESSAGE(
		false,
		FCPPT_TEXT("Tried to lock a texture which isn't there!"));
}

void
fruitcut::pp::texture::manager::clear_screen_textures()
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
		if(it->first.size() == onscreen_dim)
			textures_.erase(
				it);
	}
}

fruitcut::pp::texture::manager::~manager()
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query_internal(
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

	sge::renderer::texture::planar_ptr new_texture = 
		renderer_.create_planar_texture(
			sge::renderer::texture::planar_parameters(
				d.size(),
				d.image_format(),
				d.filter(),
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none,
				sge::renderer::texture::capabilities_field(
					sge::renderer::texture::capabilities::render_target)));

	sge::renderer::target_ptr new_target = 
		sge::renderer::target_from_texture(
			renderer_,
			*new_texture);

	switch (d.depth_stencil())
	{
		case depth_stencil_format::off:
			break;
		case depth_stencil_format::d16:
			new_target->depth_stencil_surface(
				renderer_.create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d16));
			break;
		case depth_stencil_format::d32:
			new_target->depth_stencil_surface(
				renderer_.create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d32));
			break;
		case depth_stencil_format::d24s8:
			new_target->depth_stencil_surface(
				renderer_.create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d24s8));
			break;
	}

	// There are no matching textures? Gotta create a new one!
	texture_map::iterator const result = 
		textures_.insert(
			d,
			std::auto_ptr<instance>(
				new instance(
					d,
					new_texture,
					new_target,
					true)));

	return 
		counted_instance(
			*result->second,
			std::tr1::bind(
				static_cast<void (instance::*)(bool)>(
					&instance::locked),
				std::tr1::placeholders::_1,
				false));
}
