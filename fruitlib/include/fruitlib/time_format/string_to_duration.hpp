#ifndef FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_STRING_TO_DURATION_HPP_INCLUDED

#include <fcppt/config/compiler.hpp>
#include <fcppt/optional/object.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/container/vector/vector10.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/zip_view.hpp>
#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_int.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_not_predicate.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_optional.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <chrono>
#include <string>
#include <fcppt/config/external_end.hpp>


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
				std::chrono::duration_cast<result_type>(
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
fcppt::optional::object<TargetDuration> const
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
	fcppt::optional::object<target_duration>
	optional_duration;

	// We store an optional for each duration we encounter
	typedef
	boost::fusion::vector6
	<
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

FCPPT_PP_PUSH_WARNING
#if defined(FCPPT_CONFIG_GNU_GCC_COMPILER)
FCPPT_PP_DISABLE_GCC_WARNING(-Wzero-as-null-pointer-constant)
#endif

	boost::spirit::qi::parse(
		it,
		input_string.end(),
		// The grammar. Note the rule 'm' >> !'s'. This is neccessary
		// because spirit matches "greedy"
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('h')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('m') >> !boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('m') >> boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('u') >> boost::spirit::qi::lit('s')) >>
		-(boost::spirit::qi::long_ >> boost::spirit::qi::lit('n') >> boost::spirit::qi::lit('s')),
		parse_result);

FCPPT_PP_POP_WARNING

	if(it != input_string.end())
		return optional_duration();

	typedef
	boost::fusion::vector6
	<
		std::chrono::hours,
		std::chrono::minutes,
		std::chrono::seconds,
		std::chrono::milliseconds,
		std::chrono::microseconds,
		std::chrono::nanoseconds
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
		optional_duration(
			boost::fusion::fold(
				zipped_view(
					sequence_pair(
						parse_result,
						duration_types())),
				target_duration(
					0),
				fold_functor_type()));
}
}
}

#endif
