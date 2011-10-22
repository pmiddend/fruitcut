#ifndef FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_BASE_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_BASE_HPP_INCLUDED

#include <fcppt/config/external_begin.hpp>
#include <fruitserver/program_options/detail/bad_cast.hpp>
#include <fruitserver/program_options/detail/option_holder.hpp>
#include <fruitserver/program_options/detail/option_holder_base.hpp>
#include <fruitserver/program_options/detail/option_holder_base_ptr.hpp>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <fcppt/config/external_end.hpp>


namespace fruitserver
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

#endif
