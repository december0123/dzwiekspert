#include <cstdlib>
#include <fstream>
#include <string>
#include <iostream>

extern "C" {
    #include "kiss_fft.h"
}

int main()
{
    constexpr int NFFT = 2'048;
    constexpr int FREQ = 16'000;
    kiss_fft_cfg cfg = kiss_fft_alloc(NFFT, 0, nullptr, nullptr);
    kiss_fft_cpx* buff_in = (kiss_fft_cpx*)malloc(NFFT * sizeof(*buff_in));
    kiss_fft_cpx* buff_out = (kiss_fft_cpx*)malloc(NFFT * sizeof(*buff_out));
    std::ifstream file{"/tmp/samples"};
    {
        int i = 0;
        int witam = 0;
        while(file >> witam)
        {
            buff_in[i++].r = witam;
        }
    }
    kiss_fft(cfg, buff_in, buff_out);
    std::ofstream of{"/tmp/kissfft23"};
    for (int i = 0; i < NFFT; ++i)
    {
        of << buff_out[i].r << std::endl;
    }
    free(buff_in);
    free(buff_out);
    free(cfg);
    std::cerr << "test";
}
