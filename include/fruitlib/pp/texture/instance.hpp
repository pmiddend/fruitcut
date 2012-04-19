#ifndef FRUITLIB_PP_TEXTURE_INSTANCE_HPP_INCLUDED
#define FRUITLIB_PP_TEXTURE_INSTANCE_HPP_INCLUDED

#include <fruitlib/pp/texture/descriptor.hpp>
#include <sge/renderer/target_scoped_ptr.hpp>
#include <sge/renderer/target_unique_ptr.hpp>
#include <sge/renderer/texture/planar_scoped_ptr.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/renderer/texture/planar_unique_ptr.hpp>


namespace fruitlib
{
namespace pp
{
namespace texture
{
class instance
{
FCPPT_NONCOPYABLE(
	instance);
public:
	explicit
	instance(
		texture::descriptor const &,
		sge::renderer::texture::planar_unique_ptr,
		sge::renderer::target_unique_ptr,
		bool locked);

	sge::renderer::texture::planar_shared_ptr
	texture();

	sge::renderer::target &
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
	sge::renderer::texture::planar_shared_ptr texture_;
	sge::renderer::target_scoped_ptr target_;
	bool locked_;
};
}
}
}

#endif
