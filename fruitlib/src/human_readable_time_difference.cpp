#include <fruitlib/human_readable_time_difference.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <fcppt/config/external_end.hpp>


fcppt::string const
fruitlib::human_readable_time_difference(
	std::chrono::seconds const &_seconds)
{
	typedef
	std::chrono::seconds::rep
	arithmetic_type;

	arithmetic_type const seconds =
		_seconds.count();

	arithmetic_type const minutes =
		seconds / 60;

	if(minutes < 2)
		return
			FCPPT_TEXT("Just now");

	if(minutes < 45)
		return
			fcppt::insert_to_fcppt_string(
				minutes)+
			FCPPT_TEXT(" minutes ago");

	arithmetic_type const hours =
		minutes / 60;

	if(hours < 2)
		return
			FCPPT_TEXT("An hour ago");

	if(hours < 24)
		return
			fcppt::insert_to_fcppt_string(
				hours)+
			FCPPT_TEXT(" hours ago");

	arithmetic_type const days =
		hours / 24;

	if(days < 2)
		return
			FCPPT_TEXT("A day ago");

	if(days < 7)
		return
			fcppt::insert_to_fcppt_string(
				days)+
			FCPPT_TEXT(" days ago");

	arithmetic_type const weeks =
		days / 7;

	if(weeks < 2)
		return
			FCPPT_TEXT("A week ago");

	if(weeks < 4)
		return
			fcppt::insert_to_fcppt_string(
				weeks)+
			FCPPT_TEXT(" weeks ago");

	arithmetic_type const months =
		weeks / 4;

	if(months < 2)
		return
			FCPPT_TEXT("A month ago");

	if(months < 12)
		return
			fcppt::insert_to_fcppt_string(
				months)+
			FCPPT_TEXT(" months ago");

	arithmetic_type const years =
		months / 12;

	if(years < 2)
		return
			FCPPT_TEXT("A year ago");

	return
		fcppt::insert_to_fcppt_string(
			years)+
		FCPPT_TEXT(" years ago");
}
