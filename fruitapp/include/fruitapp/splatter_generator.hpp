#ifndef FRUITAPP_SPLATTER_GENERATOR_HPP_INCLUDED
#define FRUITAPP_SPLATTER_GENERATOR_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/fruit/area.hpp>
#include <fruitapp/fruit/cut_context_fwd.hpp>
#include <fruitapp/point_sprite/color_format.hpp>
#include <fruitapp/point_sprite/system_node_fwd.hpp>
#include <fruitapp/point_sprite/splatter/acceleration.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <fruitlib/random_generator_fwd.hpp>
#include <fruitlib/uniform_int_random.hpp>
#include <fruitlib/uniform_real_random.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class splatter_generator
{
FCPPT_NONCOPYABLE(
	splatter_generator);
public:
	splatter_generator(
		sge::parse::json::object const &,
		fruitapp::point_sprite::system_node &,
		fruitlib::random_generator &,
		fruitapp::point_sprite::splatter::acceleration const &,
		fruitapp::ingame_clock const &);

	void
	fruit_was_cut(
		fruitapp::fruit::cut_context const &);

	~splatter_generator();
private:
	typedef
	fruitlib::uniform_int_random<unsigned>::type
	cut_direction_rng;

	typedef
	fruitlib::uniform_real_random<fruitapp::point_sprite::splatter::linear_velocity::value_type::value_type>::type
	speed_rng;

	typedef
	fruitlib::uniform_real_random<fruitapp::point_sprite::splatter::linear_velocity::value_type::value_type>::type
	distortion_rng;

	typedef
	fruitlib::uniform_real_random<fruitapp::point_sprite::splatter::size::value_type>::type
	size_rng;

	typedef
	fruitlib::uniform_int_random<fruitapp::point_sprite::color_format::channel_type>::type
	alpha_rng;

	typedef
	fruitlib::uniform_int_random<std::chrono::milliseconds::rep>::type
	lifetime_millis_rng;

	fruitapp::point_sprite::system_node &point_sprites_;
	fruitlib::random_generator &random_generator_;
	fruitapp::point_sprite::splatter::acceleration::value_type acceleration_;
	fruitapp::ingame_clock const &clock_;
	cut_direction_rng cut_direction_rng_;
	speed_rng speed_rng_;
	distortion_rng distortion_rng_;
	size_rng size_rng_;
	alpha_rng alpha_rng_;
	lifetime_millis_rng lifetime_millis_rng_;
	fruitapp::fruit::area const cut_area_multiplier_;
};
}

#endif
