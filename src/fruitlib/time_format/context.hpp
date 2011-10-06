#ifndef FRUITLIB_TIME_FORMAT_CONTEXT_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_CONTEXT_HPP_INCLUDED

#include <fruitlib/time_format/tags/milliseconds.hpp>
#include <fruitlib/time_format/tags/seconds.hpp>
#include <fruitlib/time_format/tags/minutes.hpp>
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/seconds.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/from_std_wstring.hpp>
#include <fcppt/math/mod.hpp>
#include <fcppt/insert_to_string.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/proto/proto.hpp>
#include <sstream>
#include <iomanip>

namespace fruitlib
{
namespace time_format
{
template<typename Duration,typename String>
class context
:
	public boost::proto::callable_context<context<Duration,String> const>
{
private:
	typedef
	fcppt::chrono::milliseconds::rep
	rep;
public:
	typedef
	String
	string;

	typedef
	Duration
	duration;

	typedef
	string
	result_type;

	explicit
	context(
		duration const &_impl)
	:
		minutes_(),
		seconds_(),
		milliseconds_()
	{
		typedef
		boost::make_unsigned<rep>::type
		urep;

		fcppt::chrono::milliseconds const d =
			fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
				_impl);

		minutes_ =
			static_cast<rep>(
				d.count() /
				static_cast<rep>(
					1000*60));
		rep const residue =
			static_cast<rep>(
				fcppt::math::mod(
					static_cast<urep>(
						d.count()),
					static_cast<urep>(
						1000*60)));
		seconds_ =
			static_cast<rep>(
				residue / static_cast<rep>(1000));
		milliseconds_ =
			static_cast<rep>(
				fcppt::math::mod(
					static_cast<urep>(residue),
					static_cast<urep>(1000)));
	}

	result_type
	operator()(
		boost::proto::tag::terminal,
		tags::milliseconds) const
	{
		std::basic_ostringstream<typename String::value_type> oss;
		oss << std::setfill(oss.widen('0')) << std::setw(2) << (milliseconds_/10);
		return oss.str();
		/*
		return
			fcppt::insert_to_string<result_type>(
				milliseconds_);
		*/
	}

	result_type
	operator()(
		boost::proto::tag::terminal,
		tags::seconds) const
	{
		return
			fcppt::insert_to_string<result_type>(
				seconds_);
	}

	result_type
	operator()(
		boost::proto::tag::terminal,
		tags::minutes) const
	{
		return
			fcppt::insert_to_string<result_type>(
				minutes_);
	}
private:
	rep minutes_,seconds_,milliseconds_;
};
}
}

#endif
