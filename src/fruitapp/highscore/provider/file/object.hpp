#ifndef FRUITAPP_HIGHSCORE_PROVIDER_FILE_OBJECT_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_FILE_OBJECT_HPP_INCLUDED

#include "../object_base.hpp"
#include "../identifier.hpp"
#include "../connection_base_ptr.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/filesystem/path.hpp>

namespace fruitapp
{
namespace highscore
{
namespace provider
{
namespace file
{
class object
:
	public provider::object_base
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		fcppt::filesystem::path const &);

	provider::identifier::value_type const
	identifier() const;

	provider::connection_base_ptr
	create_connection();

	~object();
private:
	fcppt::filesystem::path const path_;
};
}
}
}
}

#endif
