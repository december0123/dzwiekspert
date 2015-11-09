#ifndef SOUNDRECOGNIZER_HPP
#define SOUNDRECOGNIZER_HPP

#include "Note.hpp"
#include "Utils.hpp"

#include <QDebug>

#include <deque>
#include <string>
#include <vector>


class SoundRecognizer
{
public:
    SoundRecognizer();
    SoundRecognizer(const Frequency basicFreq);
    Note recognizeNote(const Frequency f) const;
    Note getRandomNote() const;
    Note findNote(const std::string name) const;
    void setBasicA(const Frequency f);

private:
    std::vector<Note> notes_;
    const double ERROR_THRESHOLD{0.029};
    const int LOW_OCTAVE{2};
    const int HIGH_OCTAVE{7};
    const int NOTES_IN_OCTAVE{12};
    Frequency basicFreq_{65.41_Hz};

    double calcRelativeError(const Note& note, const double freq) const;
    double calcFreqOfNthNoteRelativeToBasicFreq(const int N) const;
    void initNotes();
    std::deque<std::string> generateNoteNames() const;
};

#endif // SOUNDRECOGNIZER_HPP
