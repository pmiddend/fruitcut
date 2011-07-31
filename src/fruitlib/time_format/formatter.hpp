#ifndef FRUITLIB_TIME_FORMAT_FORMATTER_HPP_INCLUDED
#define FRUITLIB_TIME_FORMAT_FORMATTER_HPP_INCLUDED

#include "domain.hpp"
#include <boost/proto/proto.hpp>

namespace fruitlib
{
namespace time_format
{
template<typename Expr>
class formatter
:
	public boost::proto::extends<Expr, formatter<Expr>, domain>
{
public:
	typedef
	boost::proto::extends<Expr, formatter<Expr>, domain>
	base_type;

	formatter(Expr const &expr = Expr())
	: 
		base_type(
			expr)
	{}

	typedef	
	
};
}
}

#endif
