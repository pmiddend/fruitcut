#include "is_null.hpp"
#include <sge/parse/json/json.hpp>
#include <typeinfo>

bool
fruitcut::fruitlib::json::is_null(
	sge::parse::json::value const &v)
{
	return v.type() == typeid(sge::parse::json::null);
}
