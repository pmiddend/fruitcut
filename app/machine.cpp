#include "machine.hpp"
#include "name.hpp"
#include "events/render.hpp"
#include "events/render_overlay.hpp"
#include "events/tick.hpp"
#include "../pp/dependency_set.hpp"
#include "../json/config_wrapper.hpp"
#include "../json/find_member.hpp"
#include "../particle/sprite/parameters.hpp"
#include "../particle/objects/permanent_sprite.hpp"
#include "../media_path.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <sge/window/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/window.hpp>
#include <sge/systems/renderer.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/systems/input.hpp>
#include <sge/systems/input_helper_field.hpp>
#include <sge/systems/input_helper.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/renderer/window_parameters.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/bit_depth.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/depth_buffer.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/stencil_buffer.hpp>
#include <sge/renderer/window_mode.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/dim2.hpp>
#include <sge/renderer/caps.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/image/colors.hpp>
#include <sge/texture/rect_fragmented.hpp>
#include <sge/image2d/multi_loader.hpp>
#include <sge/console/sprite_object.hpp>
#include <sge/console/sprite_parameters.hpp>
#include <sge/image2d/file.hpp>
#include <sge/extension_set.hpp>
#include <sge/time/timer.hpp>
#include <sge/font/size_type.hpp>
#include <sge/time/unit.hpp>
#include <sge/time/second.hpp>
#include <sge/font/text/lit.hpp>
#include <sge/font/system.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <fcppt/math/dim/quad.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <boost/spirit/home/phoenix/object/new.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/object/construct.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <string>

namespace
{
void
mytest(
	bool &r)
{
	r = false;
}
}

fruitcut::app::machine::machine(
	int argc,
	char *argv[])
:
	config_file_(
		json::config_wrapper(
			fcppt::assign::make_container<std::vector<fcppt::string> >(
				FCPPT_TEXT("user_config.json")),
		argc,
		argv)),
	systems_(
		sge::systems::list()
			(sge::systems::window(
				sge::renderer::window_parameters(
					name())))
			(sge::systems::renderer(
				sge::renderer::parameters(
					sge::renderer::display_mode(
						json::find_member<sge::renderer::screen_size>(
							config_file_,
							FCPPT_TEXT("graphics/screen-size")),
						sge::renderer::bit_depth::depth32,
						sge::renderer::refresh_rate_dont_care),
					sge::renderer::depth_buffer::d24,
					sge::renderer::stencil_buffer::off,
					sge::renderer::window_mode::windowed,
					sge::renderer::vsync::on,
					sge::renderer::no_multi_sampling),
				sge::systems::viewport::manage_resize()))
			(sge::systems::input(
				sge::systems::input_helper_field(
					sge::systems::input_helper::keyboard_collector) | sge::systems::input_helper::mouse_collector)) 
			(sge::systems::parameterless::font) 
			(sge::systems::image_loader(
					sge::image::capabilities_field::null(),
					fcppt::assign::make_container<sge::extension_set>
						(FCPPT_TEXT("png"))))),
	texture_manager_(
		systems_.renderer(),
		boost::phoenix::construct<sge::texture::fragmented_unique_ptr>(
			boost::phoenix::new_<sge::texture::rect_fragmented>(
				systems_.renderer(),
				systems_.renderer()->caps().preferred_texture_format(),
				sge::renderer::filter::linear,
				fcppt::math::dim::quad<sge::renderer::dim2>(
					1024)))),
	console_object_(
		SGE_FONT_TEXT_LIT('/')),
	console_gfx_(
		console_object_,
		systems_.renderer(),
		sge::image::colors::black(),
		systems_.font_system()->create_font(
			media_path()
				/ FCPPT_TEXT("fonts")
				/ 
					json::find_member<fcppt::string>(
						config_file(),
						FCPPT_TEXT("console/font")),
			json::find_member<sge::font::size_type>(
				config_file(),
				FCPPT_TEXT("console/font-size"))),
		systems_.keyboard_collector(),
		sge::console::sprite_object(
      sge::console::sprite_parameters()
      .texture(
				create_single_texture(
					media_path()
						/ FCPPT_TEXT("textures")
						/ 
							json::find_member<fcppt::string>(
								config_file(),
								FCPPT_TEXT("console/background-texture"))))
      .pos(
        sge::console::sprite_object::point::null())
      .size(
        sge::console::sprite_object::dim(
					static_cast<sge::console::sprite_object::unit>(
						systems_.renderer()->screen_size().w()),
          static_cast<sge::console::sprite_object::unit>(
            systems_.renderer()->screen_size().h() / 2)))
      .elements()),
		json::find_member<sge::console::output_line_limit>(
			config_file(),
			FCPPT_TEXT("console/line-limit"))),
	postprocessing_(
		systems_.renderer()),
	rtt_filter_(
		systems_.renderer(),
		fcppt::math::dim::structure_cast<sge::renderer::dim2>(
			systems_.renderer()->screen_size()),
		boost::bind(
			&machine::process_event,
			this,
			events::render())),
	particle_system_(
		systems_.renderer()),
	running_(
		true),
	exit_connection_(
		systems_.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				boost::bind(
					&mytest,
					boost::ref(running_))
				/*
				boost::phoenix::ref(running_) = false*/)))
{
	postprocessing_.add_filter(
		rtt_filter_,
		FCPPT_TEXT("source"),
		fruitcut::pp::dependency_set());

	particle_system_.insert(
		particle::objects::unique_base_ptr(
			new particle::objects::permanent_sprite(
				particle::sprite::parameters()
					.visible(
						true)
					.repetition(
						json::find_member<particle::sprite::object::repetition_type>(
							config_file(),
							FCPPT_TEXT("background-repeat")))
					.rotation(
						static_cast<particle::sprite::object::rotation_type>(
							0))
					.size(
						fcppt::math::dim::structure_cast<particle::sprite::object::dim>(
							systems_.renderer()->screen_size()))
					// Texture is too big, can't use the texture manager here. Also the texture has repetition
					.texture(
						create_single_texture(	
							media_path()
								/ FCPPT_TEXT("textures")
								/ 
									json::find_member<fcppt::string>(
										config_file(),
										FCPPT_TEXT("textures/background"))))
					.pos(
						particle::sprite::object::point::null())
					.system(
						&particle_system_.sprite_system())
					.color(
						fruitcut::particle::sprite::object::color_type(
							(sge::image::color::init::red %= 1.0)
							(sge::image::color::init::green %= 1.0)
							(sge::image::color::init::blue %= 1.0)
							(sge::image::color::init::alpha %= 1.0))))));
}

sge::parse::json::object const &
fruitcut::app::machine::config_file() const
{
	return config_file_;
}

fruitcut::particle::system &
fruitcut::app::machine::particle_system()
{
	return particle_system_;
}

sge::texture::part_ptr const
fruitcut::app::machine::create_single_texture(
	fcppt::filesystem::path const &p)
{
	return 
		sge::texture::part_ptr(
			new sge::texture::part_raw(
				systems_.renderer()->create_texture(
					systems_.image_loader().load(
						p)->view(),
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none)));
}

sge::texture::part_ptr const
fruitcut::app::machine::create_texture(
	fcppt::filesystem::path const &p)
{
	return 
		sge::texture::add_image(
			texture_manager_,
			systems_.image_loader().load(
				p));
}

void
fruitcut::app::machine::run()
{
	sge::time::timer second_timer(
		sge::time::second(
			static_cast<sge::time::unit>(
				1)));

	while (running_)
	{
		systems_.window()->dispatch();

		process_event(
			events::tick(
				second_timer.reset()));

		// This implicitly sends events::render through the
		// render-to-texture filter
		postprocessing_.update();

		sge::renderer::scoped_block scoped_block(
			systems_.renderer());

		postprocessing_.render_result();

		/*
		process_event(
			events::render_overlay());

		if (!console_gfx_.active())
			continue;

		console_gfx_.draw();
		*/
	}
}
