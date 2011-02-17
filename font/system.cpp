#include "system.hpp"
#include "particle/unique_base_ptr.hpp"
#include <sge/image/colors.hpp>
#include <sge/font/bitmap/create.hpp>
#include <sge/font/system.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/container/ptr/insert_unique_ptr_map.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/move.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert_message.hpp>
#include <boost/spirit/home/phoenix/core.hpp>

fruitcut::font::system::system(
	sge::renderer::device_ptr const _renderer,
	sge::font::system_ptr _font_system,
	sge::image2d::multi_loader &_image_loader)
:
	renderer_(
		_renderer),
	font_system_(
		_font_system),
	image_loader_(
		_image_loader),
	intrusive_particles_(),
	nonintrusive_particles_(),
	bitmap_to_metrics_(),
	ttf_to_metrics_(),
	metrics_to_drawer_()
{
}

void
fruitcut::font::system::insert(
	particle::unique_base_ptr o)
{
	fcppt::container::ptr::push_back_unique_ptr(
		nonintrusive_particles_,
		fcppt::move(
			o));
}

void
fruitcut::font::system::insert(
	particle::base &o)
{
	intrusive_particles_.push_back(
		o);
}

void
fruitcut::font::system::update()
{
	for(
		nonintrusive_list::iterator i = 
			nonintrusive_particles_.begin(); 
		i != nonintrusive_particles_.end(); )
	{
		i->update();
		if (i->dead())
			i = 
				nonintrusive_particles_.erase(
					i);
		else
			++i;
	}

	for(
		intrusive_list::iterator i = 
			intrusive_particles_.begin(); 
		i != intrusive_particles_.end(); 
		++i)
		i->update();
}

void
fruitcut::font::system::render()
{
	for(
		nonintrusive_list::iterator i = 
			nonintrusive_particles_.begin(); 
		i != nonintrusive_particles_.end();
		++i)
		render(
			*i);

	for(
		intrusive_list::iterator i = 
			intrusive_particles_.begin(); 
		i != intrusive_particles_.end(); 
		++i)
		render(
			*i);
}

sge::font::metrics_ptr const
fruitcut::font::system::load_ttf(
	fcppt::filesystem::path const &p,
	sge::font::size_type const s)
{
	ttf_to_metrics::iterator cached = 
		ttf_to_metrics_.find(
			boost::make_tuple(p,s));
	if (cached == ttf_to_metrics_.end())
	{
		// insert returns pair<iterator,bool>
		cached = 
			ttf_to_metrics_.insert(
				std::make_pair(
					boost::make_tuple(
						p,
						s),
					font_system_->create_font(
						p,
						s))).first;

		create_drawer(
			cached->second);
	}
	return cached->second;
}

sge::font::metrics_ptr const
fruitcut::font::system::load_bitmap(
	fcppt::filesystem::path const &p)
{
	bitmap_to_metrics::iterator cached = 
		bitmap_to_metrics_.find(
			p);
	if (cached == bitmap_to_metrics_.end())
	{
		// insert returns pair<iterator,bool>
		cached = 
			bitmap_to_metrics_.insert(
				std::make_pair(
					p,
					sge::font::bitmap::create(
						p,
						image_loader_))).first;

		create_drawer(
			cached->second);
	}
	return cached->second;
}

fruitcut::font::system::~system()
{
}

void
fruitcut::font::system::create_drawer(
	sge::font::metrics_ptr const _metrics)
{
	fcppt::container::ptr::insert_unique_ptr_map(
		metrics_to_drawer_,
		_metrics,
		fcppt::make_unique_ptr<drawer>(
			renderer_,
			sge::image::colors::white(),
			boost::phoenix::arg_names::arg2));
}

void
fruitcut::font::system::render(
	particle::base &b)
{
	metrics_to_drawer::iterator i = 
		metrics_to_drawer_.find(
			b.metrics());

	FCPPT_ASSERT_MESSAGE(
		i != metrics_to_drawer_.end(),
		FCPPT_TEXT("Got a font particle with metrics that weren't created with the font system"));

	// This is not very nice. i->second should really be a reference
	// (but it can't), but is a pointer. Is there a function in the iterator which
	// returns a reference?
	b.render(
		*(i->second));
}
