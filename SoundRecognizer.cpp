#include "SoundRecognizer.hpp"

#include <QDebug>

#include <cmath>
#include <deque>
#include <random>
#include <vector>
#include <string>

SoundRecognizer::SoundRecognizer()
    : SoundRecognizer(440_Hz) {}

SoundRecognizer::SoundRecognizer(const Frequency basicFreq)
{
    setBasicA(basicFreq);
    initNotes();
}

void SoundRecognizer::initNotes()
{
    std::deque<std::string> noteNames{generateNoteNames()};

    for (int octave = 0; octave < HIGH_OCTAVE - LOW_OCTAVE; ++octave)
    {
        for (int note = 0; note < NOTES_IN_OCTAVE; ++note)
        {
            notes_.emplace_back(noteNames.front(),
                                calcFreqOfNthNoteRelativeToC2(note + octave * NOTES_IN_OCTAVE));
            noteNames.pop_front();
        }
    }
}

std::deque<std::string> SoundRecognizer::generateNoteNames() const
{
    std::vector<std::string> names{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    std::deque<std::string> numberedNames;
    for (int octave = LOW_OCTAVE; octave < HIGH_OCTAVE; ++octave)
    {
        for (auto& name : names)
        {
            numberedNames.emplace_back(name + std::to_string(octave));
        }
    }
    return numberedNames;
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
        return {sound->getFullName(), f, calcRelativeError(*sound, f)};
    }
    return Note::UNKNOWN();
}

Note SoundRecognizer::getRandomNote() const
{
    static std::uniform_int_distribution<> distr(0, notes_.size() - 1);
    static std::mt19937_64 eng{std::random_device{}()};
    return notes_.at(distr(eng));
}

Note SoundRecognizer::findNote(const std::string name) const
{
    auto note = std::find_if(notes_.begin(), notes_.end(),
                             [&](const Note& n)
                             {
                                return n.getFullName() == name;
                             });
    if (note != notes_.end())
    {
        return *note;
    }
    return Note::UNKNOWN();
}

void SoundRecognizer::setBasicA(const Frequency f)
{
    basicFreq_ = f / 6.72717L;
    notes_.clear();
    initNotes();
}

Note SoundRecognizer::getInInterval(std::string name, int interval) const
{
    auto note = std::find_if(notes_.begin(), notes_.end(),
                             [&](const Note& n)
                             {
                                return n.getFullName() == name;
                             });
    if (note != notes_.end())
    {
        std::advance(note, interval);
        auto distance = std::distance(notes_.begin(), note);
        if ((distance >= 0) && (distance <= static_cast<long>(notes_.size())))
        {
            return *note;
        }
    }
    return Note::UNKNOWN();
}

double SoundRecognizer::calcRelativeError(const Note &note, const double freq) const
{
    return 1.0 - (note.getFreq() / freq);
}

double SoundRecognizer::calcFreqOfNthNoteRelativeToC2(const int N) const
{
    return basicFreq_ * (std::pow(2.0, N/12.0));
}
