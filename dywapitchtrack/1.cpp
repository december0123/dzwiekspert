#include <cmath>
#include "dywapitchtrack.h"
#include <fstream>
#include <iostream>
int main()
{
    constexpr int DUPA = 4096;
    std::ifstream f{"/tmp/fft_samples"};
    //double buffer[4096];
    double* buffer = new double[DUPA];
    for (int i = 0; i < DUPA; ++i)
    {
        f >> buffer[i];
    }
    for (int i = 0; i < DUPA; ++i)
    {
        std::cout << buffer[i] << std::endl;
    }
    //Create a dywapitchtracker Object
    dywapitchtracker pitchtracker;
    //
    ////Initialise the object with this function
    dywapitch_inittracking(&pitchtracker);
    std::cout << dywapitch_computepitch(&pitchtracker, buffer, 0, DUPA);
    delete[] buffer;
}
