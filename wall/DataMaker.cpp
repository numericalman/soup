/*
   Author: Roy Bebru (RoyBebru@Gmail.Com)
*/

#define TEST_DATAMAKER

#include <algorithm> // for random_shuffle()
#include <cstdlib>  // for rand(), srand()
#include <iostream>
#include <vector>

#include "DataMaker.hpp"

using namespace std;

/* Fast generation of N=1000000 unique quite random uniformly
distributed values in the range from -V to V.
                     Y
                     ^______________
                   V :             ,--
                     :       ,-- '   :
 -500000 .   .   .   : ,-- '         :
-----:---!---!---!---O---!---!---!---:-----> X
     :         ,-- ' :    \ /     500000
     :    ,--'       :    step
     ,--'____________: -V                Pic.1

1. There exist line y = tan_coef * x from (-N/2,-V) to (N/2, V).
   Each unique x is corresponded to unique y (see Pic.1).

2. Step is segment with length 1/N (i.e. it can be 1).

3. Within step segment can be found random point x and corresponded
   unique y in range {-V,V}.

4. Random shuffled vector with such N unique y's is the searched
   data buffer.
*/

DataMaker::DataMaker(uint amount, double range_y)
{
    data.reserve(amount);

    double step = 1.L;
    double range_x = static_cast<double>(amount) / 2.L;
    if (range_x < range_y) {
        range_x = range_y;
        step = 2.L * range_x / static_cast<double>(amount);
    }
    double step_2 = step / 2.L;
    double tan_coef = range_y / range_x;
    for (uint i = 0; i < amount; ++i) {
        // Random point within next step
        double delta = step_2 / static_cast<double>(1 + rand() % (RAND_MAX - 1));
        if (rand() & 1)
            delta = -delta;
        delta += step_2;
        // Generate and push next definitely unique value
        double x = step * static_cast<double>(i) - range_x + delta;
        data.push_back(tan_coef * x);
    }
    random_shuffle(data.begin(), data.end());
}

#ifdef TEST_DATAMAKER
// $ g++ -std=c++11 -Wall <this_file>.cpp && ./a.out
// $ sort -V counter.dat | uniq > counter1.dat
// $ wc -l counter1.dat
// 1000000 counter1.dat

#include <fstream>
#include <iomanip> // setprecision(), etc
#include <ctime>

int main()
{
    srand(time(NULL)); // do not forget initialize random generator

    DataMaker buffer(1000000, 1e-15);

    ofstream file;
    file.open("counter.dat");
    for (const auto& x: buffer.data)
        file << setprecision(17)
            // << fixed 
            << x << endl;
    file.close();

    return EXIT_SUCCESS;
}

#endif
