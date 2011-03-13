#ifndef FRUITCUT_AUDIO_DETAIL_BUFFER_SEQUENCE_HPP_INCLUDED
#define FRUITCUT_AUDIO_DETAIL_BUFFER_SEQUENCE_HPP_INCLUDED

#include <sge/audio/buffer_ptr.hpp>
#include <vector>

namespace fruitcut
{
namespace audio
{
namespace detail
{
typedef
std::vector<sge::audio::buffer_ptr>
buffer_sequence;
}
}
}

#endif
