#ifndef FRUITLIB_TIME_FORMAT_CONTEXT_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_CONTEXT_HPP_INCLUDED

#include <fruitlib/time_format/tags/milliseconds.hpp>
#include <fruitlib/time_format/tags/minutes.hpp>
#include <fruitlib/time_format/tags/seconds.hpp>
#include <fcppt/from_std_wstring.hpp>
#include <fcppt/insert_to_string.hpp>
#include <fcppt/math/mod.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/proto/proto.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <iomanip>
#include <sstream>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace time_format
{
FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

template<typename Duration,typename String>
class context
:
	public boost::proto::callable_context<context<Duration,String> const>
{
private:
	typedef
	boost::chrono::milliseconds::rep
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

		boost::chrono::milliseconds const d =
			boost::chrono::duration_cast<boost::chrono::milliseconds>(
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

FCPPT_PP_POP_WARNING
}
}

#endif
