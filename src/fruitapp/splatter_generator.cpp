#include <fruitapp/splatter_generator.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/cut_context.hpp>
#include <fruitapp/fruit/triangle_traits.hpp>
#include <fruitapp/fruit/object.hpp>
#include <fruitapp/fruit/prototype.hpp>
#include <fruitapp/point_sprite/color.hpp>
#include <fruitapp/point_sprite/system_node.hpp>
#include <fruitapp/point_sprite/unique_base_ptr.hpp>
#include <fruitapp/point_sprite/splatter/object.hpp>
#include <fruitapp/point_sprite/splatter/parameters.hpp>
#include <fruitapp/point_sprite/splatter/position.hpp>
#include <fruitapp/point_sprite/splatter/linear_velocity.hpp>
#include <fruitapp/point_sprite/splatter/size.hpp>
#include <fruitlib/math/triangle/random_point.hpp>
#include <fruitlib/math/multiply_matrix4_vector3.hpp>
#include <fruitlib/uniform_random.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <fruitlib/json/parse_random_inclusive_range.hpp>
#include <sge/parse/json/config/user_config_variable.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/renderer/vector3.hpp>
#include <mizuiro/color/channel/alpha.hpp>
#include <fcppt/random/make_last_exclusive_range.hpp>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/random/make_inclusive_range.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <limits>

fruitapp::splatter_generator::splatter_generator(
	sge::parse::json::object const &config_file,
	sge::parse::json::config::user_config_variable<fruit::area::value_type> &_splatter_count_to_area_factor,
	point_sprite::system_node &_point_sprites,
	fruitlib::random_generator &_random_generator,
	point_sprite::splatter::acceleration const &_acceleration,
	fruitapp::ingame_clock const &_clock)
:
	point_sprites_(
		_point_sprites),
	random_generator_(
		_random_generator),
	acceleration_(
		_acceleration.get()),
	clock_(
		_clock),
	cut_direction_rng_(
		fcppt::random::make_inclusive_range(
			0u,
			1u),
		random_generator_),
	speed_rng_(
		fruitlib::json::parse_random_inclusive_range<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("speed-range"))),
		random_generator_),
	distortion_rng_(
		fruitlib::json::parse_random_inclusive_range<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("speed-distortion-range"))),
		random_generator_),
	size_rng_(
		fruitlib::json::parse_random_inclusive_range<sge::renderer::scalar>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("size-range"))),
		random_generator_),
	alpha_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<point_sprite::color_format::channel_type>(
				0),
			static_cast<point_sprite::color_format::channel_type>(
				std::numeric_limits<point_sprite::color_format::channel_type>::max()/2)),
		random_generator_),
	lifetime_millis_rng_(
		fruitlib::json::parse_random_inclusive_range<fcppt::chrono::milliseconds::rep>(
			sge::parse::json::find_and_convert_member<sge::parse::json::array>(
				config_file,
				sge::parse::json::path(FCPPT_TEXT("splatter-generator"))
					/ FCPPT_TEXT("lifetime-millis-range"))),
		random_generator_),
	splatter_count_to_area_factor_(
		_splatter_count_to_area_factor)
{
}

void
fruitapp::splatter_generator::fruit_was_cut(
	fruit::cut_context const &cut_info)
{
	if(cut_info.cross_section().triangles.empty())
		return;

	typedef
	fruitlib::uniform_random<fruit::mesh::triangle_sequence::size_type>::type
	triangle_rng;

	triangle_rng triangle_rng_(
		fcppt::random::make_last_exclusive_range(
			static_cast<fruit::mesh::triangle_sequence::size_type>(
				0),
			cut_info.cross_section().triangles.size()),
		random_generator_);

	typedef
	fruitlib::uniform_random<sge::renderer::scalar>::type
	triangle_point_rng;

	triangle_point_rng triangle_point_rng_(
		fcppt::random::make_inclusive_range(
			static_cast<sge::renderer::scalar>(
				0),
			static_cast<sge::renderer::scalar>(
				1)),
		random_generator_);

	for(
		unsigned
			i =
				0,
			number_of_points =
				static_cast<unsigned>(
					cut_info.area() * splatter_count_to_area_factor_.value() * 200.0);
		i < number_of_points;
		++i)
	{
		sge::renderer::vector3 const position =
			fruitlib::math::triangle::random_point(
				cut_info.cross_section().triangles[
					triangle_rng_()],
				triangle_point_rng_);

		point_sprite::color splatter_color =
			sge::image::color::any::convert<point_sprite::color::format>(
				cut_info.old().prototype().splatter_color());

		splatter_color.set(
			mizuiro::color::channel::alpha(),
			alpha_rng_());

		point_sprite::splatter::linear_velocity::value_type distortion(
			distortion_rng_(),
			distortion_rng_(),
			distortion_rng_());

		point_sprites_.push_back(
			point_sprite::unique_base_ptr(
				fcppt::make_unique_ptr<point_sprite::splatter::object>(
					point_sprite::splatter::parameters(
						point_sprites_.system(),
						point_sprite::splatter::position(
							cut_info.old().position() +
							fruitlib::math::multiply_matrix4_vector3(
								cut_info.old().world_transform(),
								position)),
						point_sprite::splatter::linear_velocity(
							distortion + ((cut_direction_rng_()
							?
								cut_info.cut_direction()
							:
								(-cut_info.cut_direction())) * speed_rng_())),
						point_sprite::splatter::acceleration(
							acceleration_),
						point_sprite::splatter::size(
							size_rng_()),
						splatter_color,
						point_sprites_.lookup_texture(
							fruitlib::resource_tree::path(
								FCPPT_TEXT("spray"))),
						fcppt::chrono::duration_cast<fruitapp::ingame_clock::duration>(
							fcppt::chrono::milliseconds(
								lifetime_millis_rng_())),
						clock_))));
	}
}

fruitapp::splatter_generator::~splatter_generator()
{
}
