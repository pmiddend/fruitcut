#ifndef FRUITCUT_GRAPHICS_SHADER_SAMPLER_HPP_INCLUDED
#define FRUITCUT_GRAPHICS_SHADER_SAMPLER_HPP_INCLUDED

#include <sge/renderer/glsl/string.hpp>
#include <sge/renderer/texture_base_ptr.hpp>

namespace fruitcut
{
namespace graphics
{
namespace shader
{
class sampler
{
public:
	typedef
	int
	texture_unit_type;

	// Initializing with an empty texture is ok, That's needed in water
	explicit
	sampler(
		sge::renderer::glsl::string const &name,
		sge::renderer::texture_base_ptr = 
			sge::renderer::texture_base_ptr());

	sge::renderer::glsl::string const
	name() const;

	sge::renderer::glsl::string const
	declaration() const;

	// The shader sets this
	void
	texture_unit(
		texture_unit_type);

	// This is needed on shader activation
	texture_unit_type
	texture_unit() const;

	// This is needed on shader activation
	sge::renderer::texture_base_ptr const
	texture() const;

	// In water, we need to update the reflection texture, so we need
	// this
	void
	texture(
		sge::renderer::texture_base_ptr);
	
	~sampler();
private:
	sge::renderer::glsl::string name_,declaration_;
	sge::renderer::texture_base_ptr texture_;
	texture_unit_type texture_unit_;
};
}
}
}

#endif
