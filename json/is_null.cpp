#include "is_null.hpp"
#include <sge/parse/json/null.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/type_traits/is_same.hpp>

namespace
{
class is_null_visitor
:
	public boost::static_visitor<bool>
{
public:
	template<typename T>
	bool
	operator()(
		T const &) const
	{
		return boost::is_same<T,sge::parse::json::null>::value;
	}
};
}

bool
fruitcut::json::is_null(
	sge::parse::json::value const &v)
{
	return 
		boost::apply_visitor(
			is_null_visitor(),
			v);
}
