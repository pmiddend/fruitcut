#include <fruitlib/exception.hpp>
#include <fruitlib/audio/music_controller.hpp>
#include <fruitlib/resource_tree/from_directory_tree.hpp>
#include <fruitlib/resource_tree/navigate_to_path.hpp>
#include <fruitlib/resource_tree/path.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/file.hpp>
#include <sge/audio/loader.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/scalar.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/nonpositional_parameters.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <sge/timer/elapsed_fractional.hpp>
#include <sge/timer/parameters.hpp>
#include <sge/timer/remaining_fractional.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/error.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <cstddef>
#include <functional>
#include <iterator>
#include <fcppt/config/external_end.hpp>


namespace
{
fcppt::shared_ptr
<
	fruitlib::uniform_int_random<std::size_t>::type
>
create_random_from_directory(
	fruitlib::random_generator &_random_generator,
	boost::filesystem::path const &p)
{
	typedef
	fruitlib::uniform_int_random<std::size_t>::type
	random_variate;

	return
		fcppt::make_shared_ptr<random_variate>(
			_random_generator,
			random_variate::distribution(
				random_variate::distribution::param_type::min(
					static_cast<std::size_t>(
						0)),
				random_variate::distribution::param_type::max(
					static_cast<std::size_t>(
						std::distance(
							boost::filesystem::directory_iterator(
								p),
							boost::filesystem::directory_iterator())-1))));
}

sge::audio::file_shared_ptr
load_shared(
	sge::audio::loader &_audio_loader,
	boost::filesystem::path const &_path)
{
	return
		sge::audio::file_shared_ptr(
			_audio_loader.load(
				_path));
}
}

fruitlib::audio::music_controller::music_controller(
	scenic::optional_parent const &_group,
	scenic::delta::callback const &_time_callback,
	fruitlib::random_generator &_random_generator,
	sge::audio::loader &_audio_loader,
	sge::audio::player &_player,
	scenic::delta::duration const &_crossfade,
	boost::filesystem::path const &_base_path,
	sge::audio::scalar const _initial_gain)
:
	node_base(
		_group),
	player_(
		_player,
		_initial_gain,
		static_cast<sge::audio::scalar>(
			1)),
	sounds_(
		fruitlib::resource_tree::from_directory_tree<resource_tree_type>(
			_base_path,
			std::bind(
				&load_shared,
				std::ref(
					_audio_loader),
				std::placeholders::_1),
			std::bind(
				&create_random_from_directory,
				std::ref(
					_random_generator),
				std::placeholders::_1))),
	clock_(
		_time_callback),
	crossfade_(
		scenic::delta::timer::parameters(
			clock_,
			_crossfade)),
	silence_buffer_(
		player_.create_buffer(
			*_audio_loader.load(
				_base_path/FCPPT_TEXT("silence.wav")))),
	silence_source_(
		silence_buffer_->create_nonpositional(
			sge::audio::sound::nonpositional_parameters())),
	current_source_(
		silence_source_),
	new_source_()
{
}

void
fruitlib::audio::music_controller::play(
	resource_tree::path const &target_path)
{
	resource_tree_type &target_tree =
		resource_tree::navigate_to_path(
			*sounds_,
			target_path);

	if(target_tree.value().is_leaf())
	{
		do_play(
			sge::audio::sound::base_shared_ptr(
				player_.create_nonpositional_stream(
					*target_tree.value().leaf_value(),
					sge::audio::sound::nonpositional_parameters())));
	}
	else
	{
		resource_tree_type &target_file =
			*boost::next(
				target_tree.begin(),
				static_cast<std::iterator_traits<resource_tree_type::const_iterator>::difference_type>(
					(*target_tree.value().node_value())()));

		if(!target_file.value().is_leaf())
			throw
				fruitlib::exception(
					FCPPT_TEXT("The argument to play() must be either a file or a directory containing just files!\nThat was not the case for: ")+
					target_path.string());

		do_play(
			sge::audio::sound::base_shared_ptr(
				player_.create_nonpositional_stream(
					*target_file.value().leaf_value(),
					sge::audio::sound::nonpositional_parameters())));
	}
}

void
fruitlib::audio::music_controller::stop()
{
	do_play(
		silence_source_);
}

sge::audio::scalar
fruitlib::audio::music_controller::gain() const
{
	return
		player_.gain();
}

void
fruitlib::audio::music_controller::gain(
	sge::audio::scalar const _gain)
{
	player_.gain(
		_gain);
}

void
fruitlib::audio::music_controller::react(
	scenic::events::update const &)
{
	clock_.update();

	if(current_source_)
		current_source_->update();

	if(!new_source_)
		return;

	if(crossfade_.expired())
	{
		FCPPT_ASSERT_ERROR(
			current_source_);
		current_source_->stop();
		current_source_ = new_source_;
		new_source_.reset();
	}
	else
	{
		current_source_->gain(
			sge::timer::remaining_fractional<sge::audio::scalar>(
				crossfade_));
		new_source_->gain(
			sge::timer::elapsed_fractional<sge::audio::scalar>(
				crossfade_));

		new_source_->update();
	}
}

fruitlib::audio::music_controller::~music_controller() {}


void
fruitlib::audio::music_controller::do_play(
	sge::audio::sound::base_shared_ptr b)
{
	b->gain(
		static_cast<sge::audio::scalar>(0));
	b->play(
		sge::audio::sound::repeat::once);

	crossfade_.reset();

	new_source_ = b;
}
