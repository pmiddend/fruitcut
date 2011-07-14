#ifndef FRUITCUT_SERVER_LEXICAL_CAST_HPP_INCLUDED
#define FRUITCUT_SERVER_LEXICAL_CAST_HPP_INCLUDED

#include <sstream>
#include <stdexcept>

namespace fruitcut
{
namespace server
{
template<typename Dest,typename Source>
Dest
lexical_cast(
	Source const &s)
{
	Dest result;
	std::stringstream ss;
	
	if (!(ss << s && ss >> result))
		throw std::runtime_error("lexical_cast failed");

	return result;
}
}
}

#endif
