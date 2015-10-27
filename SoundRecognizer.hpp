#ifndef SOUNDRECOGNIZER_HPP
#define SOUNDRECOGNIZER_HPP

#include "Note.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <cmath>
#include <deque>
#include <string>
#include <vector>
#include <QDebug>

class SoundRecognizer
{
public:
    SoundRecognizer();
    SoundRecognizer(const Frequency basicFreq);
    Note recognizeNote(const Frequency f) const;
private:
    std::vector<Note> notes_;

    const double ERROR_THRESHOLD{0.029};
    const int LOW_OCTAVE{2};
    const int HIGH_OCTAVE{7};
    const int NOTES_IN_OCTAVE{12};
    Frequency basicFreq_{65.41_Hz};

    double calcRelativeError(const Note& note, const double freq) const;
    double calcFreqOfNNoteRelativeToBasicFreq(const int N) const;

    std::deque<std::string> generateNoteNames() const;
    void initNotes();
};

#endif // SOUNDRECOGNIZER_HPP
