#include "SoundRecognizer.hpp"
#include <stdexcept>

std::string SoundRecognizer::recognizeNote(const Frequency f) const
{
    auto sound = std::find_if(notes_.begin(), notes_.end(),
                 [&](const Note& s)
                 {
                     return std::abs(1 - (s.getFreq() / f)) < 0.029;
                 });
    if (sound != notes_.end())
    {
        return sound->getName();
    }
    throw std::logic_error{"NOTE NOT RECOGNIZED"};
}
