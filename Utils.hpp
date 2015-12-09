#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

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

namespace
{

void write_little_endian(unsigned int word, int num_bytes, FILE* wav_file)
{
    unsigned buf{0};
    for (int i = 0; i < num_bytes; ++i)
    {
        buf = word & 0xFF;
        word >>= 8;
        std::fwrite(&buf, 1, 1, wav_file);
    }
}

void write_wav(std::string filename, unsigned long num_samples,
               const std::vector<unsigned int>& data, unsigned int sample_rate)
{
    constexpr unsigned int NUM_OF_CHANNELS{1};
    constexpr unsigned int BYTES_PER_SAMPLE{2};
    const unsigned int BYTE_RATE{sample_rate * NUM_OF_CHANNELS * BYTES_PER_SAMPLE};
    std::FILE* wav_file = fopen(filename.c_str(), "w");

    std::fwrite("RIFF", 1, 4, wav_file);
    write_little_endian(36 + BYTES_PER_SAMPLE * num_samples * NUM_OF_CHANNELS, 4, wav_file);
    std::fwrite("WAVE", 1, 4, wav_file);

    std::fwrite("fmt ", 1, 4, wav_file);
    write_little_endian(16, 4, wav_file);   /* SubChunk1Size is 16 */
    write_little_endian(1, 2, wav_file);    /* PCM is format 1 */
    write_little_endian(NUM_OF_CHANNELS, 2, wav_file);
    write_little_endian(sample_rate, 4, wav_file);
    write_little_endian(BYTE_RATE, 4, wav_file);
    write_little_endian(NUM_OF_CHANNELS*BYTES_PER_SAMPLE, 2, wav_file);  /* block align */
    write_little_endian(8 * BYTES_PER_SAMPLE, 2, wav_file);  /* bits/sample */

    std::fwrite("data", 1, 4, wav_file);
    write_little_endian(BYTES_PER_SAMPLE * num_samples * NUM_OF_CHANNELS, 4, wav_file);
    for (unsigned int i = 0; i < num_samples; ++i)
    {
        write_little_endian(data[i], BYTES_PER_SAMPLE, wav_file);
    }
}

void generate_wav(Frequency freq_in_hz, unsigned int duration_in_sec)
{
    const unsigned SAMPLING_RATE{44100};
    const unsigned BUF_SIZE{SAMPLING_RATE * duration_in_sec};
    std::vector<unsigned int> buffer(BUF_SIZE);
    long double amplitude{16000.0L};
    long double phase{0.0L};

    long double freq_radians_per_sample = freq_in_hz * 2 * M_PI / SAMPLING_RATE;

    /* fill buffer with a sine wave */
    for (unsigned int i = 0; i < BUF_SIZE; ++i)
    {
        phase += freq_radians_per_sample;
        buffer[i] = static_cast<unsigned int>(amplitude * std::sin(phase));
    }

    write_wav("/tmp/test.wav", BUF_SIZE, buffer, SAMPLING_RATE);
}

}

#endif // UTILS
