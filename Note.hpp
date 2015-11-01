#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <Utils.hpp>

#include <string>

class Note
{
public:
    Note()
        : name_{""}, octave_{0}, freq_{0_Hz}, error_{0.0} {}
    Note(std::string name, unsigned octave, Frequency freq)
        : name_{name}, octave_{octave}, freq_{freq}, error_{0.0} {}
    Note(std::string name, unsigned octave, Frequency freq, double error)
        : name_{name}, octave_{octave}, freq_{freq}, error_{error} {}

    Note(const Note& rhs) = default;
    Note(Note&& rhs) = default;

    Note& operator=(const Note& rhs) = default;
    Note& operator=(Note&& rhs) = default;
    ~Note() = default;

    std::string getName() const
    {
        return name_;
    }

    unsigned getOctave() const
    {
        return octave_;
    }

    std::string getFullName() const
    {
        return name_ + std::to_string(octave_);
    }

    Frequency getFreq() const
    {
        return freq_;
    }

    double getError() const
    {
        return error_;
    }

private:
    std::string name_;
    unsigned octave_{2};
    Frequency freq_;
    double error_{0.0};
};

#endif // NOTE_HPP_

