#include "SoundRecognizer.hpp"
#include <stdexcept>

Note SoundRecognizer::recognizeNote(const Frequency f) const
{
    auto sound = std::find_if(notes_.begin(), notes_.end(),
                 [&](const Note& s)
                 {
                     return std::abs(1 - (s.getFreq() / f)) < 0.029;
                 });
    if (sound != notes_.end())
    {
        return {sound->getName(), f, calcRelativeError(*sound, f)};
    }
    throw std::logic_error{"NOTE NOT RECOGNIZED"};
}

double SoundRecognizer::calcRelativeError(const Note &note, const double freq) const
{
    return 1.0 - (note.getFreq() / freq);
}
