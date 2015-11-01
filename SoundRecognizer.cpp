#include "SoundRecognizer.hpp"

#include <deque>
#include <stdexcept>
#include <string>
#include <QDebug>

SoundRecognizer::SoundRecognizer()
    : SoundRecognizer(65.41_Hz) {}

std::deque<std::pair<std::string, unsigned>> SoundRecognizer::generateNoteNames() const
{
    std::vector<std::string> names{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    std::deque<std::pair<std::string, unsigned>> numberedNames;
    for (int octave = LOW_OCTAVE; octave < HIGH_OCTAVE; ++octave)
    {
        for (auto& name : names)
        {
            numberedNames.emplace_back(name, octave);
        }
    }
    return numberedNames;
}

void SoundRecognizer::initNotes()
{
    std::deque<std::pair<std::string, unsigned>> noteNames{generateNoteNames()};

    for (int octave = 0; octave < HIGH_OCTAVE - LOW_OCTAVE; ++octave)
    {
        for (int note = 0; note < NOTES_IN_OCTAVE; ++note)
        {
            notes_.emplace_back(noteNames.front().first, noteNames.front().second,
                                calcFreqOfNNoteRelativeToBasicFreq(note + octave * NOTES_IN_OCTAVE));
            noteNames.pop_front();
        }
    }
}

SoundRecognizer::SoundRecognizer(const Frequency basicFreq)
    : basicFreq_{basicFreq}
{
    initNotes();
}

Note SoundRecognizer::recognizeNote(const Frequency f) const
{
    auto sound = std::find_if(notes_.begin(), notes_.end(),
                 [&](const Note& s)
                 {
                     return std::abs(1.0 - (s.getFreq() / f)) < ERROR_THRESHOLD;
                 });
    if (sound != notes_.end())
    {
        return {sound->getName(), sound->getOctave(), f, calcRelativeError(*sound, f)};
    }
    return {};
//    throw std::logic_error{"NOTE NOT RECOGNIZED: " + std::to_string(f)};
}

double SoundRecognizer::calcRelativeError(const Note &note, const double freq) const
{
    return 1.0 - (note.getFreq() / freq);
}

double SoundRecognizer::calcFreqOfNNoteRelativeToBasicFreq(const int N) const
{
    return basicFreq_ * (std::pow(2.0, N/12.0));
}
