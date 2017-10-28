//
//  Solver.h
//  FEM2D
//
//  Created by Macbook on 10/25/17.
//  Copyright © 2017 Trung - Việt. All rights reserved.
//

#ifndef Solver_h

#define Solver_h

#include "Geometry.h"
#include "ConjugateGradient.h"
#include "Mesh.h"
#include "GaussianQuadratures.h"
#include<iostream>

vector<Triangle> tris;
map<Point2D,long int, Point2DCompare> map_points;
bool bound_point[MAXM];
long int map_tris[MAXM][4];
long int temp1[MAXM];
bool temp2[MAXM];

using namespace std;

void FEM(function<float (float, float)> f, function<float (float, float)> u_real, int n) {
    
    cout << "Step 1: Phan hoach hinh vuong don vi thanh cac tam giac" << endl;
    
    clock_t begin_time = clock();
    
    Square square;
    square.p[0] = {0, 1};
    square.p[1] = {1, 1};
    square.p[2] = {1, 0};
    square.p[3] = {0, 0};
    
    Point2D center = {1.0f/2, 1.0f/2};
    
    //vector<Triangle> tris;
    //map<Point2D,long int, Point2DCompare> map_points;
    //bool bound_point[MAXM];
    //long int map_tris[MAXM][3];
    
    for (int i = 0; i < 4; i++) {
        int j = (i+1) % 4;
        Triangle tri = {square.p[i], center, square.p[j]};
        tris.push_back(tri);
        
        map_tris[i][1] = i;
        map_tris[i][2] = 4;
        map_tris[i][3] = j;
        
        map_points[square.p[i]] = i;
        
        bound_point[i] = true;
    }
    
    map_points[center] = 4;
    bound_point[4] = false;
    
    for (int i = 0; i < n; i++) {
        Triangulation(tris, map_points, bound_point, map_tris);
    }
    
    long int num_point_in = -1;
    
    for (map<Point2D, long int, Point2DCompare>::iterator it = map_points.begin(); it != map_points.end(); ++it) {
        
        long int index = it->second;
        
        if(!bound_point[index]) {
            num_point_in ++;
            temp1[index] = num_point_in;
        }
    }
    
    long int num_point_all = num_point_in;
    
    for (map<Point2D, long int, Point2DCompare>::iterator it = map_points.begin(); it != map_points.end(); ++it) {
        
        long int index = it->second;
        
        if(bound_point[index]) {
            num_point_all ++;
            temp1[index] = num_point_all;
        }
    }
    
    for (int i = 0; i <= num_point_all; i++) {
        temp2[i] = bound_point[i];
    }
    
    for (map<Point2D, long int, Point2DCompare>::iterator it = map_points.begin(); it != map_points.end(); ++it) {
        
        long int index = it->second;
        
        long int new_id = temp1[index];
        
        map_points[it->first] = new_id;
        
        bound_point[index] = temp2[new_id];
        
    }
    
    for (long int i = 0; i < tris.size(); i++) {
        for (int j = 1; j < 4; j++) {
            long int index = map_tris[i][j];
            map_tris[i][j] = temp1[index];
        }
    }
    
    cout << "So phan tu tam giac trong luoi la: " << tris.size() << endl;
    cout << "Thoi gian cho step 1 la: " << float (clock() - begin_time) / CLOCKS_PER_SEC << endl << endl;
    
    cout << "Step 2: Tinh Force vector F" << endl;
    begin_time = clock();
    
    vector<float> F(num_point_in + 1, 0);
    //fill(F.begin(), F.end(), 0);
    
    for (long int i = 0; i < tris.size(); i++) {
        
        Point2D p1 = tris[i].p1;
        Point2D p2 = tris[i].p2;
        Point2D p3 = tris[i].p3;
        
        long int i1 = map_tris[i][1];
        long int i2 = map_tris[i][2];
        long int i3 = map_tris[i][3];
        
        float S = Area(tris[i]);
        
        //float x = (p1.x+p2.x+p3.x)/3;
        //float y = (p1.y+p2.y+p3.y)/3;
        
         auto f_change = [p1, p2, p3, f] (float csi, float eta) {
         return f(p1.x + csi*(p2.x - p1.x) + eta*(p3.x - p1.x), p1.y + csi*(p2.y - p1.y) + eta*(p3.y - p1.y));
         };
         
         auto g1 = [f_change] (float csi, float eta){
         return f_change(csi, eta) * (1 - csi - eta);
         };
         
         auto g2 = [f_change] (float csi, float eta){
         return f_change(csi, eta) * csi;
         };
         
         auto g3 = [f_change] (float csi, float eta){
         return f_change(csi, eta) * eta;
         };
        
        //x = S * f(x,y)/3;
        
        if(!bound_point[i1]) F[i1] += 2*S*Gauss2D(g1);
        if(!bound_point[i2]) F[i2] += 2*S*Gauss2D(g2);
        if(!bound_point[i3]) F[i3] += 2*S*Gauss2D(g3);
        
    }
    
    cout << "Thoi gian cho step 2 la: " << (clock() - begin_time) / CLOCKS_PER_SEC << endl << endl;
    
    cout << "Step 3: Tinh Stiffness matrix C" << endl;
    
    begin_time = clock();
    
    map<pair<long int, long int>, float> C;
    
    for (long int i = 0; i < tris.size(); i++){
        
        Point2D p[3];
        
        p[0] = tris[i].p1;
        p[1] = tris[i].p2;
        p[2] = tris[i].p3;
        
        float S = Area(tris[i]);
        
        float A[3][3];
        
        for (int k = 0; k < 3; k++) {
            for (int l = 0; l < 3; l++) {
                A[k][l] = 0;
            }
        }
        
        for (int k = 0; k < 2; k++) {
            for (int l = k + 1; l < 3; l++) {
                float temp = (p[l].y - p[3 - k - l].y)*(p[3 - k - l].y - p[k].y) + (p[l].x - p[3 - k - l].x)*(p[3 - k - l].x - p[k].x);
                A[k][l] = temp / (2*S);
                A[l][k] = A[k][l];
            }
        }
        
        for (int k = 0; k < 3; k++) {
            int j = (k+1) % 3;
            int l = (k+2) % 3;
            float temp = (p[j].y - p[l].y)*(p[j].y - p[l].y) + (p[j].x - p[l].x)*(p[j].x - p[l].x);
            A[k][k] = temp / (2*S);
        }
        
        for (int j = 0; j < 3; j++) {
            for (int l = 0; l < 3; l++) {
                
                long int index1 = map_tris[i][j+1];
                long int index2 = map_tris[i][l+1];
                
                if (bound_point[index1] || bound_point[index2]) A[j][l] = 0;
                
                pair<long int, long int> ind;
                ind.first = index1;
                ind.second = index2;
                
                if (C.count(ind) == 0) C[ind] = 0;
                
                C[ind] += A[j][l];
            }
        }
    }
    
    cout << "Thoi gian cho step 3 la: " << float (clock() - begin_time) / CLOCKS_PER_SEC << endl << endl;
    
    cout << "Step 4: Giai he phuong trinh tuyen tinh bang phuong phap CG" << endl;
    
    begin_time = clock();
    
    for(map<pair<long int, long int>, float>::iterator it = C.begin(); it != C.end();) {
        
        float val = it->second;
        
        if(abs(val) < 1e-8) {it = C.erase(it);}
        else it++;
    }
    
    cout << "So phan tu khac 0 cua Stiffness matrix la: " << C.size() << endl;
    
    //vector<float> c(num_point_in+1, 0);
    //fill(c.begin(), c.end(), 0);
    
    vector<float> c = CGM(C, F);
    
    cout << "Thoi gian cho step 4 la: " << float (clock() - begin_time) / CLOCKS_PER_SEC << endl << endl;
    
    cout << "Step 5: Tinh sai so trong khong gian L2" << endl;
    
    begin_time = clock();
    
    float all_error = 0;
    
    for (int k = 0; k < tris.size(); k++) {
        
        Point2D p1 = tris[k].p1;
        Point2D p2 = tris[k].p2;
        Point2D p3 = tris[k].p3;
        
        float S = Area(tris[k]);
        
        long int k1 = map_tris[k][1];
        long int k2 = map_tris[k][2];
        long int k3 = map_tris[k][3];
        
        float a1 = 0;
        float a2 = 0;
        float a3 = 0;
        
        if (!bound_point[k1]) a1 = c[k1];
        if (!bound_point[k2]) a2 = c[k2];
        if (!bound_point[k3]) a3 = c[k3];
        
        auto u_real_hat = [p1, p2, p3, u_real] (float csi, float eta) {
            return u_real(p1.x + csi*(p2.x - p1.x) + eta*(p3.x - p1.x), p1.y + csi*(p2.y - p1.y) + eta*(p3.y - p1.y));
        };
         
         auto u_N_hat = [a1, a2, a3] (float csi, float eta) {
             return a1*(1 - csi - eta) + a2*csi + a3*eta;
         };
         
         auto func = [u_real_hat, u_N_hat] (float csi, float eta) {
             float tmp = (u_real_hat(csi, eta) - u_N_hat(csi, eta));
             return tmp * tmp;
         };
        
        //float a = (p1.x+p2.x+p3.x)/3;
        //float b = (p1.y+p2.y+p3.y)/3;
        //float c = f_real(a, b);
        //float d = (a1+a2+a3)/3;
        
        float val = 2*S*Gauss2D(func);
        all_error += val;
    }
    
    all_error = sqrt(all_error);
    
    cout << "Sai so trong khong gian L2 là: " << all_error << endl;
    
    cout << "Thoi gian cho step 5 la: " << float (clock() - begin_time) / CLOCKS_PER_SEC << endl << endl;
    
 }

#endif /* Solver_h */
