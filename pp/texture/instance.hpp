#ifndef FRUITCUT_PP_TEXTURE_INSTANCE_HPP_INCLUDED
#define FRUITCUT_PP_TEXTURE_INSTANCE_HPP_INCLUDED

#include "descriptor.hpp"
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>

namespace fruitcut
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
		fruitcut::pp::texture::descriptor const &,
		sge::renderer::texture_ptr,
		sge::renderer::target_ptr,
		bool locked);

	sge::renderer::texture_ptr const
	texture();

	sge::renderer::target_ptr const
	target();

	bool 
	locked() const;

	void
	locked(
		bool);

	fruitcut::pp::texture::descriptor const &
	descriptor() const;

	~instance();
private:
	fruitcut::pp::texture::descriptor descriptor_;
	sge::renderer::texture_ptr texture_;
	sge::renderer::target_ptr target_;
	bool locked_;
};
}
}
}

#endif
