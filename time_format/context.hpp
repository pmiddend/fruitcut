#ifndef FRUITCUT_TIME_FORMAT_CONTEXT_HPP_INCLUDED
#define FRUITCUT_TIME_FORMAT_CONTEXT_HPP_INCLUDED

#include "tags/milliseconds.hpp"
#include "tags/seconds.hpp"
#include <fcppt/chrono/duration_cast.hpp>
#include <fcppt/chrono/seconds.hpp>
#include <fcppt/chrono/milliseconds.hpp>
#include <fcppt/chrono/duration.hpp>
#include <fcppt/from_std_wstring.hpp>
#include <boost/lexical_cast.hpp>
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
		impl_(
			_impl)
	{
	}

	result_type
	operator()(
		boost::proto::tag::terminal,
		tags::milliseconds) const
	{
		return L"ms";
		return 
			boost::lexical_cast<result_type>(
				fcppt::chrono::duration_cast<fcppt::chrono::milliseconds>(
					impl_).count());
	}

	result_type
	operator()(
		boost::proto::tag::terminal,
		tags::seconds) const
	{
		return L"secs";
		return 
			boost::lexical_cast<result_type>(
				fcppt::chrono::duration_cast<fcppt::chrono::seconds>(
					impl_).count());
	}
private:
	duration const &impl_;
};
}
}

#endif
