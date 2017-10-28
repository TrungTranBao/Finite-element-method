//
//  GaussianQuadratures.h
//  FEM2D
//
//  Created by Macbook on 10/25/17.
//  Copyright © 2017 Trung - Việt. All rights reserved.
//

#ifndef GaussianQuadratures_h
    #define GaussianQuadratures_h
    #include<cmath>
    #include<functional>

    using namespace std;

    float Gauss2D (function<float (float, float)> g){
        return (0.5f)* ((9.0f/40) * g(1.0f/3, 1.0f/3)
                    +((155 - sqrt(15)) / 1200)*g((6 - sqrt(15))*(1.0f/21), (1.0f/21)*(6 - sqrt(15)))
                    +((155 - sqrt(15)) / 1200)*g((9 + 2*sqrt(15))*(1.0f/21), (6 - sqrt(15))*(1.0f/21))
                    +((155 - sqrt(15)) / 1200)*g((6 - sqrt(15))*(1.0f/21), (9 + 2*sqrt(15))*(1.0f/21))
                    +((155 + sqrt(15)) / 1200)*g((6 + sqrt(15))*(1.0f/21), (6 + sqrt(15))*(1.0f/21))
                    +((155 + sqrt(15)) / 1200)*g((6 + sqrt(15))*(1.0f/21), (9 - 2*sqrt(15))*(1.0f/21))
                    +((155 + sqrt(15)) / 1200)*g((9 - 2*sqrt(15))*(1.0f/21), (6 + sqrt(15))*(1.0f/21)));
        }

#endif /* GaussianQuadratures_h */
