#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <Utils.hpp>

#include <string>
#include <iostream>
class Note
{
public:
    Note()
        : name_{""}, freq_{0_Hz}, error_{0.0} {}
    Note(std::string name, Frequency freq)
        : name_{name}, freq_{freq}, error_{0.0} {}
    Note(std::string name, Frequency freq, double error)
        : name_{name}, freq_{freq}, error_{error} {}

    Note(const Note& rhs) = default;
    Note(Note&& rhs) = default;

    Note& operator=(const Note& rhs) = default;
    Note& operator=(Note&& rhs) = default;
    ~Note() = default;

    std::string getName() const
    {
        return {name_.begin(), name_.end() - 1};
    }

    std::string getFullName() const
    {
        return name_;
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
    Frequency freq_;
    double error_{0.0};
};

#endif // NOTE_HPP_

