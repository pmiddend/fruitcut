#include "manager.hpp"
#include "instance.hpp"
#include "counted_instance.hpp"
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/bind.hpp>
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
		renderer_->create_target(
			new_texture,
			sge::renderer::no_depth_stencil_texture());

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

fruitcut::pp::texture::manager::~manager()
{
}
