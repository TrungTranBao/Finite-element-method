//
//  ConjugateGradient.h
//  FEM2D
//
//  Created by Macbook on 10/25/17.
//  Copyright © 2017 Trung - Việt. All rights reserved.
//

#ifndef ConjugateGradient_h

    #define ConjugateGradient_h

    #include "Geometry.h"
    #include<iostream>

    vector<float> r1, r2, p;

    using namespace std;

    vector<float> CGM(map<pair<long int, long int>, float> &A, vector<float> b) {
        
        vector<float> x(b.size(), 0);
        
        
        r1 = b;
        r2 = b;
        p = b;
        
         long int count = 0;
        
        float beta1 = Dot(r2, r2);
        
        for(int i = 1; i < b.size(); i++){
            count++;
            
            if(i > 1){
                beta1 = Dot(r2, r2);
                float beta = beta1/ Dot(r1, r1);
                p = Add(r2, Mul(p, beta));
            }
            
            vector<float> temp = Mul(A, p);
            
            float anpha = beta1 / Dot(p, temp);
            x = Add(x, Mul(p, anpha));
            r1 = r2;
            r2 = Sub(r2, Mul(temp, anpha));
            
            if(Dot(r2, r2) < 1e-10) break;
        }
        
        cout << "So lan lap su dung phuong phap CG la: " << count << endl;
        return x;
    }

#endif /* ConjugateGradient_h */
