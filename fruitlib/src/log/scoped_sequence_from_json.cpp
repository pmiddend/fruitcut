#include <fruitlib/exception.hpp>
#include <fruitlib/log/scoped.hpp>
#include <fruitlib/log/scoped_sequence_from_json.hpp>
#include <fruitlib/log/string_to_location.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/element_vector.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/log/context.hpp>
#include <fcppt/log/level_from_string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitlib::log::scoped_sequence
fruitlib::log::scoped_sequence_from_json(
	fcppt::log::context &context,
	sge::parse::json::array const &a)
{
	scoped_sequence result;

	try
	{
		for(
			sge::parse::json::element_vector::const_iterator r =
				a.elements.begin();
			r != a.elements.end();
			++r)
		{
			sge::parse::json::array const &current_pair =
				sge::parse::json::get<sge::parse::json::array>(
					*r);

			if(current_pair.elements.size() != 2)
				throw exception(
					FCPPT_TEXT("The loggers have to be stored in an array of pairs [name,level], this was not the case with at least one logger pair"));

			result.push_back(
				fcppt::make_unique_ptr<scoped>(
					context,
					string_to_location(
						FCPPT_TEXT("sge/")+sge::parse::json::get<fcppt::string>(
							current_pair.elements[0]),
						FCPPT_TEXT('/')),
					fcppt::log::level_from_string(
						sge::parse::json::get<fcppt::string>(
							current_pair.elements[1]))));
		}
	}
	catch(sge::parse::json::invalid_get const &)
	{
		throw exception(
			FCPPT_TEXT("The loggers have to be stored in an array of pairs [name,level], this was not the case with at least one logger pair"));
	}

	return
		std::move(
			result);
}
