#ifndef FRUITCUT_SANDBOX_SPLAT_COLLECTOR_HPP_INCLUDED
#define FRUITCUT_SANDBOX_SPLAT_COLLECTOR_HPP_INCLUDED

#include <fruitlib/particle/sprite/system.hpp>
#include <fruitlib/particle/sprite/object.hpp>
#include <fruitlib/particle/sprite/parameters.hpp>
#include <fruitlib/pp/screen_vf/quad.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/sprite/object_impl.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/shader/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <vector>

namespace fruitcut
{
namespace sandbox
{
class splat_collector
{
FCPPT_NONCOPYABLE(
	splat_collector);
public:
	explicit
	splat_collector(
		sge::renderer::device &);

	sge::renderer::texture::planar_ptr const
	texture();

	void
	insert(
		fruitlib::particle::sprite::parameters);

	void
	update();

	~splat_collector();
private:
	typedef
	std::vector<fruitlib::particle::sprite::object>
	sprite_sequence;

	sge::renderer::device &renderer_;
	// texture_ is what the user gets
	// temp_texture_ is the texture we use to merge the "previous"
	// texture and the new sprites
	sge::renderer::texture::planar_ptr texture_,temp_texture_;
	sge::renderer::target_ptr texture_target_,temp_texture_target_;
	fruitlib::particle::sprite::system ss_;
	fruitlib::particle::sprite::object background_;
	sprite_sequence sprites_;
	sge::shader::object copy_shader_;
	fruitlib::pp::screen_vf::quad quad_;
};
}
}

#endif
