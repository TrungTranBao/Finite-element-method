//
//  main.cpp
//  FEM2D
//
//  Created by Macbook on 10/25/17.
//  Copyright © 2017 Trung - Việt. All rights reserved.
//

#include <iostream>
#include "Solver.h"

using namespace std;

double f(double x, double y) {
    return 2*x*(1 - x) + 2*y*(1 - y);
    
}

double u_real(double x, double y) {
    return x*y*(1-x)*(1-y);
}

int main() {
    
    cout << "CHUONG TRINH GIAI PHUONG TRINH ELIPTIC BIEN DIRICHLET BANG FEM" << endl << endl << endl << endl;
    clock_t begin_time = clock();
    
    FEM(f, u_real, 6);
    
    cout << "Tong thoi gian chay chuong trinh " << (clock() - begin_time) / CLOCKS_PER_SEC << endl;
    
    return 0;
}
