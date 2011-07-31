#include <fruitlib/json/output_tabbed.hpp>
#include <fruitlib/exception.hpp>
#include <sge/parse/json/json.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>
#include <fcppt/io/ostringstream.hpp>
#include <fcppt/string.hpp>
#include <fcppt/format.hpp>
#include <fcppt/nonassignable.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/lexical_cast.hpp>

namespace
{
struct output_visitor
{
FCPPT_NONASSIGNABLE(
	output_visitor);
public:
	typedef 
	fcppt::string 
	result_type;

	typedef
	fcppt::string::size_type
	tab_count;

	explicit
	output_visitor(
		tab_count const _tabs)
	:
		tabs_(
			_tabs)
	{
	}

	result_type
	operator()(
		sge::parse::json::object const &o) const
	{
		fcppt::io::ostringstream stream;

		stream 
			<< (fcppt::format(FCPPT_TEXT("%s{\n")) % make_tabs()).str();

		for(
			sge::parse::json::member_vector::const_iterator i = 
				o.members.begin();
			i != o.members.end();
			++i)
		{
			stream <<
				(fcppt::format(FCPPT_TEXT("%s\"%s\" : \n")) % make_more_tabs() % i->name).str();
			
			stream << 
				boost::apply_visitor(
					output_visitor(
						tabs_+2),
					i->value);

			if(i != --o.members.end())
				stream << FCPPT_TEXT(",");
			stream << FCPPT_TEXT("\n");
		}
		stream << make_tabs() << FCPPT_TEXT("}");

		return stream.str();
	}

	result_type
	operator()(
		sge::parse::json::array const &o) const
	{
		fcppt::io::ostringstream stream;

		stream 
			<< (fcppt::format(FCPPT_TEXT("%s[\n")) % make_tabs()).str();

		for(
			sge::parse::json::element_vector::const_iterator i = 
				o.elements.begin();
			i != o.elements.end();
			++i)
		{
			stream << 
				boost::apply_visitor(
					output_visitor(
						tabs_+1),
					*i);

			if(i != --o.elements.end())
				stream << FCPPT_TEXT(",");
			stream << FCPPT_TEXT("\n");
		}
		stream << make_tabs() << FCPPT_TEXT("]");

		return stream.str();
	}

	result_type
	operator()(
		sge::parse::json::string const &o) const
	{
		if(o.find(FCPPT_TEXT('\"')) != sge::parse::json::string::npos)
			throw fruitlib::exception(FCPPT_TEXT("quoted strings are unsupported"));
		return make_tabs()+FCPPT_TEXT("\"")+o+FCPPT_TEXT("\"");
	}

	result_type
	operator()(
		bool const o) const
	{
		return 
			make_tabs()+(
			o
			?
				fcppt::string(FCPPT_TEXT("true"))
			:
				fcppt::string(FCPPT_TEXT("false")));
	}

	result_type
	operator()(
		sge::parse::json::null const &) const
	{
		return make_tabs()+FCPPT_TEXT("null");
	}

	result_type
	operator()(
		sge::parse::json::int_type const o) const
	{
		return make_tabs()+boost::lexical_cast<result_type>(o);
	}

	result_type
	operator()(
		sge::parse::json::float_type const o) const
	{
		return make_tabs()+(fcppt::format(FCPPT_TEXT("%1$.2f")) % o).str();
	}
private:
	tab_count const tabs_;

	fcppt::string
	make_tabs() const
	{
		return fcppt::string(tabs_,FCPPT_TEXT('\t'));
	}

	fcppt::string
	make_more_tabs() const
	{
		return fcppt::string(tabs_+1,FCPPT_TEXT('\t'));
	}

	fcppt::string
	make_even_more_tabs() const
	{
		return fcppt::string(tabs_+2,FCPPT_TEXT('\t'));
	}
};
}

fcppt::string const
fruitlib::json::output_tabbed(
	sge::parse::json::object const &o)
{
	sge::parse::json::value v(
		o);

	return 
		boost::apply_visitor(
			output_visitor(
				0),
			v);
}
