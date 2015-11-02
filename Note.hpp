#ifndef NOTE_HPP_
#define NOTE_HPP_

#include <Utils.hpp>

#include <string>
#include <iostream>
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
    ~Note() = default;

    std::string getName() const
    {
        return {fullName_.begin(), fullName_.end() - 1};
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

private:
    std::string fullName_;
    Frequency freq_;
    double error_{0.0};
};

#endif // NOTE_HPP_

