#include "system.hpp"
#include <sge/sprite/default_equal.hpp>
#include <sge/config/media_path.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/glsl/istream_ref.hpp>
#include <sge/renderer/glsl/program.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cifstream.hpp>

namespace
{
sge::renderer::glsl::program_ptr const
create_point_shader(
	sge::renderer::device_ptr const rend)
{
	fcppt::io::cifstream fragment_stream(
		sge::config::media_path()
		/ FCPPT_TEXT("shaders")
		/ FCPPT_TEXT("pointsprite_fragment.glsl"));

	fcppt::io::cifstream vertex_stream(
		sge::config::media_path()
		/ FCPPT_TEXT("shaders")
		/ FCPPT_TEXT("pointsprite_vertex.glsl"));

	return 
		rend->create_glsl_program(
			sge::renderer::glsl::istream_ref(
				vertex_stream),
			sge::renderer::glsl::istream_ref(
				fragment_stream));
}
}

fruitcut::particle::system::system(
	sge::renderer::device_ptr const _renderer)
:
	pss_(
		_renderer),
	ss_(
		_renderer),
	particles_(),
	point_shader_(
		create_point_shader(
			_renderer)),
	point_shader_texture_(
		point_shader_->uniform(
			"tex"))
{
}

void
fruitcut::particle::system::update()
{
	for(particle_sequence::iterator i = particles_.begin(); i != particles_.end();)
	{
		i->update();
		if (i->dead())
			i = 
				particles_.erase(
					i);
		else
			++i;
	}
}

void 
fruitcut::particle::system::render()
{
	ss_.render_all(
		sge::sprite::default_equal());

	sge::renderer::glsl::scoped_program scoped_shader(
		pss_.renderer(),
		point_shader_);
	pss_.render_all(
		sge::sprite::default_equal());
}

fruitcut::particle::sprite::system &
fruitcut::particle::system::sprite_system()
{
	return ss_;
}

fruitcut::particle::point_sprite::system &
fruitcut::particle::system::point_sprite_system()
{
	return pss_;
}

void 
fruitcut::particle::system::insert(
	objects::unique_base_ptr o)
{
	fcppt::container::ptr::push_back_unique_ptr(
		particles_,
		fcppt::move(
			o));
}

fruitcut::particle::system::~system()
{
}
