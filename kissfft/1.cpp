#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <memory>
extern "C" {
    #include "kiss_fft.h"
}

struct freeDelete
{
    void operator()(void* p) {free(p);}
};

int main()
{
    constexpr int NFFT = 2'048;
    constexpr int FREQ = 16'000;
    std::unique_ptr<kiss_fft_state, freeDelete> cfg{kiss_fft_alloc(NFFT, 0, nullptr, nullptr)};
    std::vector<kiss_fft_cpx> buff_in(NFFT);
    std::vector<kiss_fft_cpx> buff_out(NFFT);
    std::ifstream file{"/tmp/samples"};
    std::cerr << "1" << std::endl;
    {
        int i = 0;
        int witam = 0;
    std::cerr << "2" << std::endl;
        while(file >> witam)
        {
            buff_in[i++].r = witam;
    std::cerr << i << std::endl;
        }
    }
    kiss_fft(cfg.get(), buff_in.data(), buff_out.data());
    std::ofstream of{"/tmp/kissfft23"};
    for (int i = 0; i < NFFT; ++i)
    {
        of << buff_out[i].r << std::endl;
    }
    std::cerr << "test";
}
