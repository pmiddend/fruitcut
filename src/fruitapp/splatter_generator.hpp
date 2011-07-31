#ifndef FRUITAPP_SPLATTER_GENERATOR_HPP_INCLUDED
#define FRUITAPP_SPLATTER_GENERATOR_HPP_INCLUDED

#include <fruitapp/fruit/cut_context_fwd.hpp>
#include <fruitapp/fruit/area.hpp>
#include <fruitlib/uniform_random.hpp>
#include <fruitlib/random_generator.hpp>
#include <fruitlib/json/user_config_variable.hpp>
#include <fruitapp/point_sprite/color_format.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/acceleration.hpp>
#include <fruitapp/point_sprite/system_node_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/time/callback.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/time/unit.hpp>
#include <fcppt/noncopyable.hpp>

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
		fruitlib::json::user_config_variable<fruit::area::value_type> &_splatter_count_to_area_factor,
		point_sprite::system_node &,
		fruitlib::random_generator &,
		point_sprite::splatter::acceleration const &,
		sge::time::callback const &);

	void
	fruit_was_cut(
		fruit::cut_context const &);

	~splatter_generator();
private:
	typedef
	fruitlib::uniform_random<unsigned>::type
	cut_direction_rng;

	typedef
	fruitlib::uniform_random<point_sprite::splatter::linear_velocity::value_type::value_type>::type
	speed_rng;

	typedef
	fruitlib::uniform_random<point_sprite::splatter::linear_velocity::value_type::value_type>::type
	distortion_rng;

	typedef
	fruitlib::uniform_random<point_sprite::splatter::size::value_type>::type
	size_rng;

	typedef
	fruitlib::uniform_random<point_sprite::color_format::channel_type>::type
	alpha_rng;

	typedef
	fruitlib::uniform_random<sge::time::unit>::type
	lifetime_millis_rng;

	point_sprite::system_node &point_sprites_;
	fruitlib::random_generator &random_generator_;
	point_sprite::splatter::acceleration::value_type acceleration_;
	sge::time::callback const time_callback_;
	cut_direction_rng cut_direction_rng_;
	speed_rng speed_rng_;
	distortion_rng distortion_rng_;
	size_rng size_rng_;
	alpha_rng alpha_rng_;
	lifetime_millis_rng lifetime_millis_rng_;
	fruitlib::json::user_config_variable<fruit::area::value_type> &splatter_count_to_area_factor_;
};
}

#endif
