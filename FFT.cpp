#include "FFT.hpp"
#include <memory>

FFTBuffer FFT::run(const int NFFT, const FFTBuffer &inputBuffer)
{
    std::unique_ptr<kiss_fft_state, FreeDeleter> state{kiss_fft_alloc(NFFT, 0, nullptr, nullptr)};
    FFTBuffer outputBuffer(NFFT);
    kiss_fft(state.get(), inputBuffer.getData(), outputBuffer.getData());
    return outputBuffer;
}
