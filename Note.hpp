#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <string>
#include <Utils.hpp>

class Note
{
public:
    Note(std::string name, Frequency freq)
        : name_{name}, freq_{freq} {}
    std::string getName() const
    {
        return name_;
    }
    Frequency getFreq() const
    {
        return freq_;
    }

private:
    const std::string name_;
    const Frequency freq_;
};

#endif // NOTE_HPP_

