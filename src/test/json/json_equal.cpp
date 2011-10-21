#include "json_equal.hpp"
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/int_type.hpp>
#include <sge/parse/json/member.hpp>
#include <sge/parse/json/null.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/value.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>
#include <algorithm>
#include <cmath>
#include <typeinfo>
#include <fcppt/config/external_end.hpp>


namespace
{
class equality_visitor
{
FCPPT_NONASSIGNABLE(
	equality_visitor);
public:
	typedef
	bool
	result_type;

	explicit
	equality_visitor(
		sge::parse::json::value const &_other)
	:
		other_(
			_other)
	{
	}

	template<typename T>
	bool
	operator()(
		T const &t) const
	{
		return
			other_.type() == typeid(t) &&
			fruitcut::test::json::json_equal(
				boost::get<T>(
					other_),
				t);
	}
private:
	sge::parse::json::value const &other_;
};
}

bool
fruitcut::test::json::json_equal(
	sge::parse::json::null const &,
	sge::parse::json::null const &)
{
	return true;
}

bool
fruitcut::test::json::json_equal(
	sge::parse::json::object const &a,
	sge::parse::json::object const &b)
{
	return
		a.members.size() == b.members.size() &&
		std::equal(
			a.members.begin(),
			a.members.end(),
			b.members.begin(),
			static_cast<bool (*)(sge::parse::json::member const &,sge::parse::json::member const &)>(
				&json_equal));
}

bool
fruitcut::test::json::json_equal(
	sge::parse::json::array const &a,
	sge::parse::json::array const &b)
{
	return
		a.elements.size() == b.elements.size() &&
		std::equal(
			a.elements.begin(),
			a.elements.end(),
			b.elements.begin(),
			static_cast<bool (*)(sge::parse::json::value const &,sge::parse::json::value const &)>(
				&json_equal));
}

bool
fruitcut::test::json::json_equal(
	fcppt::string const &a,
	fcppt::string const &b)
{
	return a == b;
}

bool
fruitcut::test::json::json_equal(
	bool const a,
	bool const b)
{
	return a == b;
}

bool
fruitcut::test::json::json_equal(
	sge::parse::json::int_type const a,
	sge::parse::json::int_type const b)
{
	return a == b;
}

bool
fruitcut::test::json::json_equal(
	sge::parse::json::float_type const a,
	sge::parse::json::float_type const b)
{
	sge::parse::json::float_type const diff =
		std::abs(
			a - b);

	return
		diff < static_cast<sge::parse::json::float_type>(0.001);
}

bool
fruitcut::test::json::json_equal(
	sge::parse::json::value const &a,
	sge::parse::json::value const &b)
{
	return
		a.which() == b.which() &&
		boost::apply_visitor(
			equality_visitor(
				b),
			a);
}

bool
fruitcut::test::json::json_equal(
	sge::parse::json::member const &a,
	sge::parse::json::member const &b)
{
	return a.name == b.name && json_equal(a.value,b.value);
}
