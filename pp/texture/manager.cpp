#include "manager.hpp"
#include "instance.hpp"
#include "use_screen_size.hpp"
#include "counted_instance.hpp"
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/target_from_texture.hpp>
#include <sge/renderer/depth_stencil_format.hpp>
#include <sge/renderer/target.hpp>
#include <sge/renderer/texture/planar_parameters.hpp>
#include <sge/renderer/texture/planar.hpp>
#include <sge/renderer/texture/address_mode.hpp>
#include <sge/renderer/texture/address_mode2.hpp>
#include <sge/renderer/texture/filter/linear.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/image/color/format.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/bind.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/text.hpp>
#include <iostream>
#include <utility>
#include <memory>

fruitcut::pp::texture::manager::manager(
	sge::renderer::device_ptr const _renderer)
:
	renderer_(
		_renderer),
	textures_(),
	screen_textures_()
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query(
	descriptor const &d)
{
	return 
		query_internal(
			d.size() == use_screen_size()
			?
				screen_textures_
			:
				textures_,
			descriptor(
				d.size() == use_screen_size()
				?
					fcppt::math::dim::structure_cast<sge::renderer::dim2>(
						renderer_->onscreen_target()->viewport().get().dimension())
				:
					d.size(),
				d.image_format(),
				d.filter(),
				d.depth_stencil()));
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query(
	sge::renderer::texture::planar_ptr const t)
{
	FCPPT_ASSERT(
		t);

	for (texture_map::iterator i = textures_.begin(); i != textures_.end(); ++i)
	{
		if (i->second->texture() == t)
		{
			i->second->locked(
				true);
			return 
				counted_instance(
					*(i->second),
					boost::bind(
						&instance::locked,
						_1,
						false));
		}
	}

	FCPPT_ASSERT_MESSAGE(
		false,
		FCPPT_TEXT("Tried to lock a texture which isn't there!"));
}

fruitcut::pp::texture::manager::~manager()
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query_internal(
	texture_map &target_map,
	descriptor const &d)
{
	boost::iterator_range<texture_map::iterator> eq_range = 
		target_map.equal_range(
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
				boost::bind(
					&instance::locked,
					_1,
					false));
	}

	std::cout << "adding to map " << &target_map << "\n";

	sge::renderer::texture::planar_ptr new_texture = 
		renderer_->create_planar_texture(
			sge::renderer::texture::planar_parameters(
				d.size(),
				d.image_format(),
				d.filter(),
				sge::renderer::texture::address_mode2(
					sge::renderer::texture::address_mode::clamp),
				sge::renderer::resource_flags::none));

	sge::renderer::target_ptr new_target = 
		sge::renderer::target_from_texture(
			renderer_,
			new_texture);

	switch (d.depth_stencil())
	{
		case depth_stencil_format::off:
			break;
		case depth_stencil_format::d16:
			new_target->depth_stencil_surface(
				renderer_->create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d16));
			break;
		case depth_stencil_format::d32:
			new_target->depth_stencil_surface(
				renderer_->create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d32));
			break;
		case depth_stencil_format::d24s8:
			new_target->depth_stencil_surface(
				renderer_->create_depth_stencil_surface(
					d.size(),
					sge::renderer::depth_stencil_format::d24s8));
			break;
	}

	// There are no matching textures? Gotta create a new one!
	texture_map::iterator const result = 
		target_map.insert(
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
			boost::bind(
				&instance::locked,
				_1,
				false));
}
