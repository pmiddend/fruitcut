#include "manager.hpp"
#include "instance.hpp"
#include "counted_instance.hpp"
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/target_from_texture.hpp>
#include <sge/renderer/depth_stencil_format.hpp>
#include <sge/renderer/target.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/bind.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/text.hpp>
#include <utility>
#include <memory>

fruitcut::pp::texture::manager::manager(
	sge::renderer::device_ptr const _renderer)
:
	renderer_(
		_renderer)
{
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query(
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
				boost::bind(
					&instance::locked,
					_1,
					false));
	}

	sge::renderer::texture_ptr new_texture = 
		renderer_->create_texture(
			d.size(),
			d.image_format(),
			d.filter(),
			sge::renderer::resource_flags::none);

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
			boost::bind(
				&instance::locked,
				_1,
				false));
}

fruitcut::pp::texture::counted_instance const
fruitcut::pp::texture::manager::query(
	sge::renderer::texture_ptr const t)
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
