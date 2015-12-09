#ifndef UTILS
#define UTILS

using Frequency = long double;

constexpr Frequency operator "" _Hz(unsigned long long freq)
{
    return freq;
}

constexpr Frequency operator "" _Hz(long double freq)
{
    return freq;
}

template<typename T>
constexpr T PI = T(3.141592653589793238513L);

#endif // UTILS
