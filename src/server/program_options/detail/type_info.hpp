#ifndef FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_TYPE_INFO_HPP_INCLUDED
#define FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_TYPE_INFO_HPP_INCLUDED

#include <typeinfo>

namespace fruitcut
{
namespace server
{
namespace program_options
{
namespace detail
{
class type_info
{
public:
	type_info(
		std::type_info const &);

	std::type_info const &
	get() const;
private:
	std::type_info const *info_;
};
}
}
}
}

#endif
