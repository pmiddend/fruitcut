#ifndef FRUITCUT_SANDBOX_PP_FILTER_BASE_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_FILTER_BASE_HPP_INCLUDED

#include "property.hpp"
#include <boost/unordered/unordered_map.hpp>
#include <fcppt/string.hpp>
#include <fcppt/assert.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
namespace filter
{
class base
{
public:
	typedef
	boost::unordered_map
	<
		fcppt::string,
		property
	>
	property_map;

	explicit
	base(
		property_map const & = property_map());

	virtual 
	~base();
protected:
	template<typename T>
	T const 
	get(
		fcppt::string const &s)
	{
		FCPPT_ASSERT(properties_.find(s) != properties_.end());
		return properties_[s].get<T>();
	}
private:
	property_map properties_;
};
}
}
}
}

#endif
