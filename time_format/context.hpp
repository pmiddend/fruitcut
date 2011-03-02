#ifndef FRUITCUT_TIME_FORMAT_CONTEXT_HPP_INCLUDED
#define FRUITCUT_TIME_FORMAT_CONTEXT_HPP_INCLUDED

#include "tags/milliseconds.hpp"
#include "tags/seconds.hpp"
#include "tags/minutes.hpp"
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/seconds.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/from_std_wstring.hpp>
#include <fcppt/math/mod.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/proto/proto.hpp>

namespace fruitcut
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
		return 
			boost::lexical_cast<result_type>(
				milliseconds_);
	}

	result_type
	operator()(
		boost::proto::tag::terminal,
		tags::seconds) const
	{
		return 
			boost::lexical_cast<result_type>(
				seconds_);
	}

	result_type
	operator()(
		boost::proto::tag::terminal,
		tags::minutes) const
	{
		return 
			boost::lexical_cast<result_type>(
				minutes_);
	}
private:
	rep minutes_,seconds_,milliseconds_;
};
}
}

#endif
