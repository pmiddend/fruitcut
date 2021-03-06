#include <fruitlib/average_colors.hpp>
#include <sge/image2d/view/const_object.hpp>
#include <mizuiro/no_init.hpp>
#include <mizuiro/color/for_each_channel.hpp>
#include <mizuiro/color/object.hpp>
#include <mizuiro/color/output.hpp>
#include <mizuiro/color/format/homogenous_static.hpp>
#include <mizuiro/color/types/channel_value.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/type_traits/integral_promotion.hpp>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <fcppt/config/external_end.hpp>


namespace
{
struct view_visitor
{
public:
	typedef
	sge::image::color::any::object
	result_type;

	template<typename View>
	result_type
	operator()(
		View const &) const;
};

// TODO: Pattern match on mizuiro::color::homogenous?
template<typename Input>
struct promote_homogenous_channels
{
	typedef
	mizuiro::color::format::homogenous_static
	<
		typename boost::integral_promotion<typename Input::channel_type>::type,
		typename Input::layout
	>
	type;
};

template<typename Color>
struct set_to_zero_functor
{
FCPPT_NONASSIGNABLE(
	set_to_zero_functor);
public:
	typedef
	void
	result_type;

	explicit
	set_to_zero_functor(
		Color &_target_color)
	:
		target_color_(
			_target_color)
	{

	}

	template<typename Channel>
	result_type
	operator()(
		Channel const &c) const
	{
		typedef
		mizuiro::color::types::channel_value<typename Color::format,Channel>
		channel_value_type;

		target_color_.set(
			c,
			static_cast<channel_value_type>(0));
	}
private:
	Color &target_color_;
};

template<typename Color>
Color const
zero_initialized()
{
	Color result{
		mizuiro::no_init{}
	};

	mizuiro::color::for_each_channel(
		result,
		set_to_zero_functor<Color>(
			result));
	return result;
}

template<typename SourceColor,typename DestColor>
struct addition_functor
{
FCPPT_NONASSIGNABLE(
	addition_functor);
public:
	typedef
	void
	result_type;

	explicit
	addition_functor(
		SourceColor const &_source,
		DestColor &_dest)
	:
		source_(
			_source),
		dest_(
			_dest)
	{
	}

	template<typename Channel>
	result_type
	operator()(
		Channel const &c) const
	{
		dest_.set(
			c,
			static_cast<typename DestColor::format::channel_type>(
				dest_.get(
					c) +
				source_.get(
					c)));
	}
private:
	SourceColor const &source_;
	DestColor &dest_;
};

template<typename SourceColor,typename DestColor>
struct division_functor
{
FCPPT_NONASSIGNABLE(
	division_functor);
public:
	typedef
	void
	result_type;

	explicit
	division_functor(
		SourceColor const &_source,
		DestColor &_dest,
		typename SourceColor::format::channel_type const _divisor)
	:
		source_(
			_source),
		dest_(
			_dest),
		divisor_(
			_divisor)
	{
	}

	template<typename Channel>
	result_type
	operator()(
		Channel const &c) const
	{
		typedef
		mizuiro::color::types::channel_value<typename DestColor::format,Channel>
		destination_channel_type;

		dest_.set(
			c,
			static_cast<destination_channel_type>(
				source_.get(
					c)/
				divisor_));
	}
private:
	SourceColor const &source_;
	DestColor &dest_;
	typename SourceColor::format::channel_type const divisor_;
};

template<typename SourceColor,typename DestColor>
addition_functor<SourceColor,DestColor> const
make_addition_functor(
	SourceColor const &source,
	DestColor &dest)
{
	return
		addition_functor<SourceColor,DestColor>(
			source,
			dest);
}

template<typename View>
view_visitor::result_type
view_visitor::operator()(
	View const &v) const
{
	typedef typename
	View::format::color_format
	color_format;

	typedef typename
	promote_homogenous_channels<color_format>::type
	promoted_color_format;

	typedef
	mizuiro::color::object<promoted_color_format>
	promoted_color;

	typedef
	mizuiro::color::object<color_format>
	input_color;

	typedef typename
	View::iterator
	iterator;

	promoted_color result(
		zero_initialized<promoted_color>());

	typedef typename
	promoted_color_format::channel_type
	promoted_channel_type;

	promoted_channel_type
		number_of_pixels = 0;
	for(
		iterator current_pixel = v.begin();
		current_pixel != v.end();
		++current_pixel)
	{
		mizuiro::color::for_each_channel(
			result,
			make_addition_functor(
				*current_pixel,
				result));
		++number_of_pixels;
	}

	input_color narrowed_result{
		mizuiro::no_init{}
	};

	mizuiro::color::for_each_channel(
		result,
		division_functor<promoted_color,input_color>(
			result,
			narrowed_result,
			number_of_pixels));

	return
		sge::image::color::any::object(
			narrowed_result);
}
}

sge::image::color::any::object const
fruitlib::average_colors(
	sge::image2d::view::const_object const &input_view)
{
	return
		fcppt::variant::apply_unary(
			view_visitor(),
			input_view.get());
}
