#ifndef FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_OPTION_BASE_HPP_INCLUDED
#define FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_OPTION_BASE_HPP_INCLUDED

#include "option_holder_base_ptr.hpp"
#include "bad_cast.hpp"
#include "option_holder_base.hpp"
#include "option_holder.hpp"
#include <stdexcept>
#include <typeinfo>
#include <string>

namespace fruitcut
{
namespace server
{
namespace program_options
{
namespace detail
{
class option_base
{
public:	
	explicit
	option_base(
		std::string const &,
		option_holder_base_ptr);

	std::string const &
	name() const;

	template<typename T>
	T const
	get()
	{
		if(holder_->type().get() != typeid(T))
			throw 
				detail::bad_cast(
					"Invalid type cast (expected "+
					std::string(
						typeid(T).name())+
					", got "+
					holder_->type().get().name()+
					")");

		return 
			static_cast<option_holder<T> *>(
				holder_.get())->get();
	}

	option_holder_base_ptr const 
	holder() const;
private:
	std::string name_;
	option_holder_base_ptr holder_;
};
}
}
}
}

#endif
