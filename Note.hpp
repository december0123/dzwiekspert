#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <string>
#include <Utils.hpp>

class Note
{
public:
    Note()
        : name_{""}, freq_{0_Hz} {}
    Note(std::string name, Frequency freq)
        : name_{name}, freq_{freq} {}
    Note(const Note& rhs) = default;
    Note(Note&& rhs) = default;

    Note& operator=(const Note& rhs) = default;
    Note& operator=(Note&& rhs) = default;

    std::string getName() const
    {
        return name_;
    }
    Frequency getFreq() const
    {
        return freq_;
    }

private:
    std::string name_;
    Frequency freq_;
};

#endif // NOTE_HPP_

