//
//  Geometry.h
//  FEM2D
//
//  Created by Macbook on 10/25/17.
//  Copyright © 2017 Trung - Việt. All rights reserved.
//

#ifndef Geometry_h

    #define Geometry_h

    #include<vector>
    #include<map>
    #include<cmath>
    #include<iostream>

    using namespace std;

    struct Point2D {
        float x, y;
    };

    struct Point2DCompare{
        bool operator() (const Point2D p1, const Point2D p2) const {
            return ((p1.x < p2.x)||((p1.x == p2.x)&&(p1.y < p2.y)));
        }
    };

    Point2D Mid(Point2D p1, Point2D p2) {
        Point2D p;
        p.x = (p1.x + p2.x) / 2;
        p.y = (p1.y + p2.y) / 2;
        return p;
    }

    float Distance(Point2D p1, Point2D p2) {
        return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    }

    struct Triangle {
        Point2D p1, p2, p3;
    };

    float Area(Triangle tri) {
        float a = Distance(tri.p2, tri.p3);
        float b = Distance(tri.p3, tri.p1);
        float c = Distance(tri.p1, tri.p2);
        
        float p = (a + b + c)/2;
        
        return sqrt(p*(p - a)*(p - b)*(p - c));
    }

    struct Square {
        Point2D p[4];
    };

    vector<float> Sub(vector<float> a, vector<float> b) {
        for (int i = 0; i < a.size(); i++){
            a[i] = a[i] - b[i];
        }
        return a;
    }

    vector<float> Add(vector<float> a, vector<float> b) {
        for (int i = 0; i < a.size(); i++){
            a[i] = a[i] + b[i];
        }
        return a;
    }

    float Dot(vector<float> a, vector<float> b) {
        float res = 0;
        
        for(int i = 0; i < a.size(); i++) {
            res += a[i]*b[i];
        }
        return res;
    }

    vector<float> Mul(vector<float> a, float b) {
        for(int i = 0; i < a.size(); i++) {
            a[i] = a[i]*b;
        }
        return a;
    }

    void Display(vector<float> a) {
        for (int i = 0; i < a.size(); i++) {
            cout << a[i] << " ";
        }
        cout << endl;
    }

    vector<float> Mul(map<pair<long int, long int>, float> &A, vector<float> &b) {
        vector<float> res(b.size(), 0);
        for(map<pair<long int, long int>, float>::iterator it = A.begin(); it != A.end(); ++it) {
            
            long int x = it->first.first;
            long int y = it->first.second;
            if(y > b.size()) cout << "Boo" << endl;
            
            float val = it->second;
            
            res[x-1] += val*b[y-1];
        }
        return res;
    }

#endif /* Geometry_h */
