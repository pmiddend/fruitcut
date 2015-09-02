#ifndef FRUITLIB_FONT_MANAGER_HPP_INCLUDED
#define FRUITLIB_FONT_MANAGER_HPP_INCLUDED

#include <fruitlib/texture_manager_fwd.hpp>
#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/font/base_path.hpp>
#include <fruitlib/font/cache_fwd.hpp>
#include <sge/font/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/unique_ptr.hpp>


namespace fruitlib
{
namespace font
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	FRUITLIB_DETAIL_SYMBOL
	manager(
		sge::renderer::device::ffp &,
		sge::renderer::texture::emulate_srgb,
		sge::font::system &,
		fruitlib::texture_manager &,
		sge::parse::json::object const &,
		fruitlib::font::base_path const &);

	sge::renderer::device::ffp &
	renderer() const;

	sge::renderer::texture::emulate_srgb
	emulate_srgb() const;

	fruitlib::font::cache &
	cache();

	FRUITLIB_DETAIL_SYMBOL
	~manager();
private:
	sge::renderer::device::ffp &renderer_;
	sge::renderer::texture::emulate_srgb const emulate_srgb_;
	// Pimpl
	fcppt::unique_ptr<fruitlib::font::cache> const cache_;
};
}
}

#endif
