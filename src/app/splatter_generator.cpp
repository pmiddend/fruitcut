#include "splatter_generator.hpp"
#include "fruit/mesh.hpp"
#include "fruit/cut_context.hpp"
#include "fruit/triangle_traits.hpp"
#include "fruit/object.hpp"
#include "point_sprite/color.hpp"
#include "point_sprite/system_node.hpp"
#include "point_sprite/unique_base_ptr.hpp"
#include "point_sprite/splatter/object.hpp"
#include "point_sprite/splatter/parameters.hpp"
#include "point_sprite/splatter/position.hpp"
#include "point_sprite/splatter/linear_velocity.hpp"
#include "point_sprite/splatter/size.hpp"
#include "../math/triangle/random_point.hpp"
#include "../math/multiply_matrix4_vector3.hpp"
#include "../uniform_random.hpp"
#include "../create_rng.hpp"
#include "../json/find_member.hpp"
#include "../json/parse_random_inclusive_range.hpp"
#include <sge/renderer/scalar.hpp>
#include <sge/parse/json/json.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/image/color/any/convert.hpp>
#include <mizuiro/color/channel/alpha.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <limits>

fruitcut::app::splatter_generator::splatter_generator(
	sge::parse::json::object const &config_file,
	point_sprite::system_node &_point_sprites,
	point_sprite::splatter::acceleration const &_acceleration,
	sge::time::callback const &_time_callback)
:
	point_sprites_(
		_point_sprites),
	acceleration_(
		_acceleration.get()),
	time_callback_(
		_time_callback),
	cut_direction_rng_(
		fcppt::random::make_inclusive_range(
			0u,
			1u),
		fruitcut::create_rng()),
	speed_rng_(
		json::parse_random_inclusive_range<sge::renderer::scalar>(
			json::find_member<sge::parse::json::array>(
				config_file,
				FCPPT_TEXT("splatter-generator/speed-range"))),
		fruitcut::create_rng()),
	size_rng_(
		json::parse_random_inclusive_range<sge::renderer::scalar>(
			json::find_member<sge::parse::json::array>(
				config_file,
				FCPPT_TEXT("splatter-generator/size-range"))),
		fruitcut::create_rng()),
	alpha_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<point_sprite::color_format::channel_type>(
				0),
			static_cast<point_sprite::color_format::channel_type>(
				std::numeric_limits<point_sprite::color_format::channel_type>::max()/2)),
		fruitcut::create_rng()),	
	lifetime_millis_rng_(
		json::parse_random_inclusive_range<sge::time::unit>(
			json::find_member<sge::parse::json::array>(
				config_file,
				FCPPT_TEXT("splatter-generator/lifetime-millis-range")))),
	splatter_count_to_area_factor_(
		json::find_member<sge::renderer::scalar>(
			config_file,
			FCPPT_TEXT("splatter-generator/splatter-count-to-area-factor")))
{
}

void
fruitcut::app::splatter_generator::fruit_was_cut(
	fruit::cut_context const &cut_info)
{
	if(cut_info.cross_section().triangles.empty())
		return;

	typedef
	fruitcut::uniform_random<fruit::mesh::triangle_sequence::size_type>::type
	triangle_rng;

	triangle_rng triangle_rng_(
		fcppt::random::make_last_exclusive_range(
			static_cast<fruit::mesh::triangle_sequence::size_type>(
				0),
			cut_info.cross_section().triangles.size()),
		fruitcut::create_rng());

	typedef
	fruitcut::uniform_random<sge::renderer::scalar>::type
	triangle_point_rng;

	triangle_point_rng triangle_point_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				1)));

	for(
		unsigned 
			i = 
				0,
			number_of_points = 
				static_cast<unsigned>(
					cut_info.area() * splatter_count_to_area_factor_);
		i < number_of_points; 
		++i)
	{
		sge::renderer::vector3 const position = 
			math::triangle::random_point(
				cut_info.cross_section().triangles[
					triangle_rng_()],
				triangle_point_rng_);

		point_sprite::color splatter_color = 
			sge::image::color::any::convert<point_sprite::color::format>(
				cut_info.old().splatter_color());

		splatter_color.set<mizuiro::color::channel::alpha>(
			static_cast<point_sprite::color_format::channel_type>(
				128));

		point_sprites_.push_back(
			point_sprite::unique_base_ptr(
				fcppt::make_unique_ptr<point_sprite::splatter::object>(
					point_sprite::splatter::parameters(
						point_sprites_.system(),
						point_sprite::splatter::position(
							cut_info.old().position() + 
							math::multiply_matrix4_vector3(
								cut_info.old().world_transform(),
								position)),
						point_sprite::splatter::linear_velocity(
							(cut_direction_rng_() 
							? 
								cut_info.cut_direction() 
							: 
								(-cut_info.cut_direction())) * speed_rng_()),
						point_sprite::splatter::acceleration(
							acceleration_),
						point_sprite::splatter::size(
							size_rng_()),
						splatter_color,
						point_sprites_.lookup_texture(
							FCPPT_TEXT("splat0")),
						sge::time::millisecond(
							lifetime_millis_rng_()),
						time_callback_))));
	}
}

fruitcut::app::splatter_generator::~splatter_generator()
{
}
