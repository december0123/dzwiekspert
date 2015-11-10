#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <Utils.hpp>

#include <algorithm>
#include <string>
#include <vector>

class Note
{
public:
    Note()
        : fullName_{""}, freq_{0_Hz}, error_{0.0L} {}
    Note(std::string name, Frequency freq)
        : fullName_{name}, freq_{freq}, error_{0.0L} {}
    Note(std::string name, Frequency freq, double error)
        : fullName_{name}, freq_{freq}, error_{error} {}

    Note(const Note& rhs) = default;
    Note(Note&& rhs) = default;

    Note& operator=(const Note& rhs) = default;
    Note& operator=(Note&& rhs) = default;
    bool operator==(const Note& rhs)
    {
        return fullName_ == rhs.fullName_;
    }

    ~Note() = default;

    std::string getName() const
    {
        if (fullName_.size())
        {
            return {fullName_.begin(), fullName_.end() - 1};
        }
        return fullName_;
    }

    std::string getFullName() const
    {
        return fullName_;
    }

    Frequency getFreq() const
    {
        return freq_;
    }

    double getError() const
    {
        return error_;
    }

    static Note UNKNOWN()
    {
        return Note{"UNKNOWN", 0_Hz, 1};
    }

private:
    std::string fullName_;
    Frequency freq_;
    double error_{0.0};
    std::vector<std::string> names_{"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
};

#endif // NOTE_HPP_

