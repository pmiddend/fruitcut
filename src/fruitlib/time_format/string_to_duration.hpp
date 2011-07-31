#ifndef FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_HPP_INCLUDED

#include <fcppt/chrono/chrono.hpp>
#include <fcppt/optional.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/container/vector/vector10.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <string>

namespace fruitlib
{
namespace time_format
{
namespace detail
{
template<typename TargetDuration>
struct duration_fold_functor
{
	typedef
	TargetDuration
	result_type;

	template<typename OptionalRepDurationPair>
	result_type const
	operator()(
		TargetDuration const &accumulated_duration,
		OptionalRepDurationPair const &oprep_and_duration) const
	{
		// This is either a "DurationType&" or "DurationType const &"
		// because we're operating on a zip_view
		typedef typename 
		boost::fusion::result_of::at
		<
			OptionalRepDurationPair,
			boost::mpl::int_<0>
		>::type
		optional_rep;

		// This is boost::optional<long> (note the remove_reference, else
		// the code below breaks)
		typedef typename 
		boost::remove_cv
		<
			typename
			boost::remove_reference
			<
				typename
				boost::fusion::result_of::at
				<
					OptionalRepDurationPair,
					boost::mpl::int_<1>
				>::type
			>::type
		>::type
		source_duration;

		optional_rep rep(
			boost::fusion::at_c<0>(
				oprep_and_duration));

		return 
			rep
			?
				accumulated_duration + 
				fcppt::chrono::duration_cast<result_type>(
					source_duration(
						*rep))
			:
				accumulated_duration;
	}
};
}

template
<
	typename TargetDuration,
	typename Char,
	typename Traits
>
fcppt::optional<TargetDuration> const
string_to_duration(
	std::basic_string<Char,Traits> const &input_string)
{
	typedef 
	std::basic_string<Char,Traits>
	string;

	typedef typename
	string::const_iterator
	string_iterator;

	typedef
	TargetDuration
	target_duration;

	typedef
	fcppt::optional<target_duration>
	optional_duration;

	// We store an optional for each duration we encounter
	typedef
	boost::fusion::vector7
	<
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>,
		boost::optional<long>
	>
	parse_result_type;
	// TODO: y u no work!?
	/*
	typedef
	boost::array<boost::optional<long>,7>
	parse_result_type;
	*/

	string_iterator it = 
		input_string.begin();

	parse_result_type parse_result;

	boost::spirit::qi::parse(
		it,
		input_string.end(),
		// The grammar. Note the rule 'm' >> !'s'. This is neccessary
		// because spirit matches "greedy"
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('d')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('h')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('m') >> !boost::spirit::qi::lit('s')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('s')) >> 
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('m') >> boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('u') >> boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('n') >> boost::spirit::qi::lit('s')),
		parse_result);

	if(it != input_string.end())
		return optional_duration();

	typedef
	boost::fusion::vector7
	<
		fcppt::chrono::days,
		fcppt::chrono::hours,
		fcppt::chrono::minutes,
		fcppt::chrono::seconds,
		fcppt::chrono::milliseconds,
		fcppt::chrono::microseconds,
		fcppt::chrono::nanoseconds
	>
	duration_types;

	typedef
	boost::fusion::vector2
	<
		parse_result_type const &,
		duration_types const &
	>
	sequence_pair;
	
	typedef
	boost::fusion::zip_view<sequence_pair>
	zipped_view;

	typedef 
	detail::duration_fold_functor<target_duration>
	fold_functor_type;

	// This is really tricky: We take the optional<duration> tuple and
	// zip it with the chrono::durations, so we get pairs
	// "(optional<duration>,chrono::duration)". These pairs we iterate
	// over and accumulate the total duration
	return 
		boost::fusion::fold(
			zipped_view(
				sequence_pair(
					parse_result,
					duration_types())),
			target_duration(
				0),
			fold_functor_type());
}
}
}

#endif
