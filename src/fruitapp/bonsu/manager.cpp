#include <fruitapp/bonsu/manager.hpp>
#include <fruitapp/bonsu/scalar.hpp>
#include <fruitapp/bonsu/instance_wrapper.hpp>
#include <fruitapp/bonsu/rectangle/manager.hpp>
#include <fruitapp/bonsu/rectangle/padding.hpp>
#include <fruitlib/json/find_and_convert_member.hpp>
#include <fruitlib/json/string_to_path.hpp>
#include "../../media_path.hpp"
#include <sge/sprite/default_equal.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/onscreen_target.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <cstdlib>
#include <utility>

fruitapp::bonsu::manager::manager(
	fruitlib::scenic::optional_parent const &_parent,
	sge::parse::json::object const &_config_file,
	sge::image2d::multi_loader &_image_loader,
	sge::renderer::device &_renderer)
:
	node_base(
		_parent),
	renderer_(
		_renderer),
	texture_manager_(
		_renderer,
		_image_loader),
	sprite_system_(
		_renderer),
	rectangle_speed_(
		fruitlib::json::find_and_convert_member<bonsu::scalar>(
			_config_file,
			fruitlib::json::string_to_path(
				FCPPT_TEXT("bonsu/manager/rectangle-speed")))),
	rectangle_bounding_rect_height_(
		fruitlib::json::find_and_convert_member<bonsu::scalar>(
			_config_file,
			fruitlib::json::string_to_path(
				FCPPT_TEXT("bonsu/manager/rectangle-height-fraction")))),
	rectangle_padding_left_(
		fruitlib::json::find_and_convert_member<bonsu::scalar>(
			_config_file,
			fruitlib::json::string_to_path(
				FCPPT_TEXT("bonsu/manager/rectangle-padding-left-fraction")))),
	rectangle_padding_top_(
		fruitlib::json::find_and_convert_member<bonsu::scalar>(
			_config_file,
			fruitlib::json::string_to_path(
				FCPPT_TEXT("bonsu/manager/rectangle-padding-top-fraction")))),
	rectangle_padding_middle_(
		fruitlib::json::find_and_convert_member<bonsu::scalar>(
			_config_file,
			fruitlib::json::string_to_path(
				FCPPT_TEXT("bonsu/manager/rectangle-padding-middle-fraction")))),
	rectangle_manager_(),
	bonsu_()
{
	react(
		fruitlib::scenic::events::viewport_change());
}

fruitapp::bonsu::manager::~manager()
{
}

void
fruitapp::bonsu::manager::react(
	fruitlib::scenic::events::update const &)
{
	for(
		bonsu_list::iterator it = 
			bonsu_.begin();
		it != bonsu_.end();
		++it)
		it->update();
}

void
fruitapp::bonsu::manager::react(
	fruitlib::scenic::events::render const &)
{
	sprite_system_.render_all(
		sge::sprite::default_equal());
}

void
fruitapp::bonsu::manager::react(
	fruitlib::scenic::events::viewport_change const &)
{
	rectangle::manager::rect const viewport = 
		fcppt::math::box::structure_cast<rectangle::manager::rect>(
			renderer_.onscreen_target().viewport().get());

	// A bit hacky, but ok.
	if(viewport.size().content() < static_cast<bonsu::scalar>(0.1f))
		return;

	rectangle_manager_.take(
		fcppt::make_unique_ptr<rectangle::manager>(
			rectangle::manager::rect(
				viewport.pos(),
				rectangle::manager::rect::dim(
					viewport.size().w(),
					viewport.size().h() * rectangle_bounding_rect_height_)),
			rectangle::padding(
				viewport.size().w() * rectangle_padding_left_,
				viewport.size().h() * rectangle_padding_top_,
				viewport.size().h() * rectangle_padding_middle_),
			rectangle_speed_));
	
	// Why would this be necessary?
	for(
		bonsu_list::iterator it = 
			bonsu_.begin();
		it != bonsu_.end();
		++it)
		it->update();
}

void
fruitapp::bonsu::manager::insert(
	instance::base &new_instance)
{
	fcppt::container::ptr::push_back_unique_ptr(
		bonsu_,
		fcppt::make_unique_ptr<bonsu::instance_wrapper>(
			fcppt::ref(
				new_instance),
			fcppt::ref(
				sprite_system_),
			fcppt::ref(
				texture_manager_)));
}

void
fruitapp::bonsu::manager::erase(
	instance::base &b)
{
	for(
		bonsu_list::iterator it = 
			bonsu_.begin();
		it != bonsu_.end();
		++it)
	{
		if(&(it->instance()) == &b)
		{
			bonsu_.erase(
				it);
			return;
		}
	}

	std::abort();
}
