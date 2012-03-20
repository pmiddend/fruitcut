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
#include <sge/parse/json/config/user_config_variable.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/chrono/milliseconds.hpp>


namespace fruitapp
{
class splatter_generator
{
FCPPT_NONCOPYABLE(
	splatter_generator);
public:
	explicit
	splatter_generator(
		sge::parse::json::object const &,
		sge::parse::json::config::user_config_variable<fruit::area::value_type> &_splatter_count_to_area_factor,
		point_sprite::system_node &,
		fruitlib::random_generator &,
		point_sprite::splatter::acceleration const &,
		fruitapp::ingame_clock const &);

	void
	fruit_was_cut(
		fruit::cut_context const &);

	~splatter_generator();
private:
	typedef
	fruitlib::uniform_int_random<unsigned>::type
	cut_direction_rng;

	typedef
	fruitlib::uniform_real_random<point_sprite::splatter::linear_velocity::value_type::value_type>::type
	speed_rng;

	typedef
	fruitlib::uniform_real_random<point_sprite::splatter::linear_velocity::value_type::value_type>::type
	distortion_rng;

	typedef
	fruitlib::uniform_real_random<point_sprite::splatter::size::value_type>::type
	size_rng;

	typedef
	fruitlib::uniform_int_random<point_sprite::color_format::channel_type>::type
	alpha_rng;

	typedef
	fruitlib::uniform_int_random<fcppt::chrono::milliseconds::rep>::type
	lifetime_millis_rng;

	point_sprite::system_node &point_sprites_;
	fruitlib::random_generator &random_generator_;
	point_sprite::splatter::acceleration::value_type acceleration_;
	fruitapp::ingame_clock const &clock_;
	cut_direction_rng cut_direction_rng_;
	speed_rng speed_rng_;
	distortion_rng distortion_rng_;
	size_rng size_rng_;
	alpha_rng alpha_rng_;
	lifetime_millis_rng lifetime_millis_rng_;
	sge::parse::json::config::user_config_variable<fruit::area::value_type> &splatter_count_to_area_factor_;
};
}

#endif
