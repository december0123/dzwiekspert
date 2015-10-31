#include <cmath>
#include "dywapitchtrack.h"
#include <fstream>
#include <iostream>
#include <memory>
int main()
{
    constexpr int DUPA = 4096;
    std::ifstream f{"/tmp/samples"};
    //double buffer[4096];
    //std::unique_ptr<double[]> buffer = std::make_unique<double[]>(DUPA);
    double* buffer = (double*)malloc(DUPA * sizeof(double));
    for (int i = 0; i < DUPA; ++i)
    {
        f >> buffer[i];
    }
    //Create a dywapitchtracker Object
    dywapitchtracker pitchtracker;
    //
    ////Initialise the object with this function
    dywapitch_inittracking(&pitchtracker);
    while(true)
    std::cout << dywapitch_computepitch(&pitchtracker, buffer, 0, DUPA);
}
