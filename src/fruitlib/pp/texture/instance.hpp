#ifndef FRUITLIB_PP_TEXTURE_INSTANCE_HPP_INCLUDED
#define FRUITLIB_PP_TEXTURE_INSTANCE_HPP_INCLUDED

#include <fruitlib/pp/texture/descriptor.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>

namespace fruitlib
{
namespace pp
{
namespace texture
{
class instance
{
public:
	explicit
	instance(
		texture::descriptor const &,
		sge::renderer::texture::planar_ptr,
		sge::renderer::target_ptr,
		bool locked);

	sge::renderer::texture::planar_ptr const
	texture();

	sge::renderer::target_ptr const
	target();

	bool 
	locked() const;

	void
	locked(
		bool);

	texture::descriptor const &
	descriptor() const;

	~instance();
private:
	texture::descriptor descriptor_;
	sge::renderer::texture::planar_ptr texture_;
	sge::renderer::target_ptr target_;
	bool locked_;
};
}
}
}

#endif
