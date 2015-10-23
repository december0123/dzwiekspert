#ifndef UTILS
#define UTILS

using Frequency = long double;

constexpr unsigned long long operator "" _Hz(unsigned long long freq)
{
    return freq;
}

constexpr long double operator "" _Hz(long double freq)
{
    return freq;
}

#endif // UTILS

