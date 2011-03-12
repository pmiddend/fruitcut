#include "animated.hpp"
#include "../drawer.hpp"
#include "../scoped_transformation.hpp"
#include <sge/font/text/draw.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/dim.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/tr1/functional.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <iostream>

namespace
{
sge::font::rect const
scale_transformation(
	sge::font::rect const &total_rect,
	sge::font::rect const &character_rect,
	fruitcut::font::scale_animation::value_type::second_type const s)
{
	typedef
	fruitcut::font::scale_animation::value_type::second_type
	real;

	sge::font::pos const c = 
		total_rect.pos() + total_rect.size()/2;
	return 
		sge::font::rect(
			sge::font::pos(
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						c.x()) - 
					static_cast<real>(
						c.x()) * s + 
					static_cast<real>(
						character_rect.pos().x()) * s),
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						c.y()) - 
					static_cast<real>(
						c.y()) * s + 
					static_cast<real>(
						character_rect.pos().y()) * s)),
			sge::font::dim(
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						character_rect.size().w()) * s),
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						character_rect.size().h()) * s)));
}
}

fruitcut::font::particle::animated::animated(
	base_parameters const &params,
	color_animation::value_sequence const &_color_animation,
	scale_animation::value_sequence const &_scale_animation,
	sge::time::callback const &cb)
:
	base(
		params),
	color_animation_(
		_color_animation,
		cb),
	scale_animation_(
		_scale_animation,
		cb)
{
}

void
fruitcut::font::particle::animated::update()
{
	color_animation_.update();
	scale_animation_.update();
}

void
fruitcut::font::particle::animated::render(
	sge::font::metrics_ptr const _metrics,
	drawer &_drawer)
{
	_drawer.color(
		color_animation_.current_value());

	scoped_transformation scoped_tr(
		_drawer,
		std::tr1::bind(
			&scale_transformation,
			std::tr1::placeholders::_1,
			std::tr1::placeholders::_2,
			scale_animation_.current_value()));

	base::render(
		_metrics,
		_drawer);
}

bool
fruitcut::font::particle::animated::dead() const
{
	return color_animation_.finished();
}

fruitcut::font::particle::animated::~animated()
{
}
