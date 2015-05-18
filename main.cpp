//
//  main.cpp
//  metopt
//
//  Created by luba_yaronskaya on 24.03.15.
//  Copyright (c) 2015 LiubovMIPT. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <future>
#include "svd.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    std::filebuf fb;
    std::istream in(fb.open("/Users/luba_yaronskaya/Documents/kinopoisk_res.txt", std::ios::in));
     
    u_long users_cnt, products_cnt;
    in >> users_cnt >> products_cnt;
    typedef std::vector<std::vector<double>> matrix_t;

    matrix_t R(users_cnt, std::vector<double>(products_cnt, 0));

    u_long n, features;
    
    in >> n >> features;
    
    for (u_long i = 0; i < n; ++i)
    {
        int user, product, value;
        in >> user >> product >> value;
        R[user][product] = value;
    }
    fb.close();

    matrix_t U(users_cnt, std::vector<double>(features, 0));
    matrix_t V(products_cnt, std::vector<double>(features, 0));

    SWD<double> swd_worker;
    swd_worker.run(R, U, V, features);
    swd_worker.print_decomposed();

    return 0;
}





