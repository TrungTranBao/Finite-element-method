//
//  Mesh.h
//  FEM2D
//
//  Created by Macbook on 10/25/17.
//  Copyright © 2017 Trung - Việt. All rights reserved.
//

#ifndef Mesh_h

    #define Mesh_h

    #include"Geometry.h"

    /*const int MAXN = 5*1e3;*/
    const long int MAXM = 6*1e6;

    using namespace std;

    void Triangulation(vector<Triangle> &tris, map<Point2D, long int, Point2DCompare> &map_points,
                       bool bound_point[], long int map_tris[][4]) {
        
        long int old_m = tris.size();
        long int new_m = 0;
        
        long int new_n = map_points.size();
        
        vector<Triangle> new_tris;
        
        for (long int i = 0; i < old_m; i++) {
            for (int j = 1; j < 4; j++) {
                map_tris[i][j] = 0;
            }
        }
        
        for(long int i = 0; i < old_m; i++) {
            Triangle tri = tris[i];
            
            Point2D p1 = tri.p1;
            Point2D p2 = tri.p2;
            Point2D p3 = tri.p3;
            
            Point2D mid1 = Mid(p2, p3);
            Point2D mid2 = Mid(p3, p1);
            Point2D mid3 = Mid(p1, p2);
            
            if(map_points.count(mid1) == 0) {
                new_n ++;
                map_points[mid1] = new_n - 1;
            }
            if(map_points.count(mid2) == 0) {
                new_n ++;
                map_points[mid2] = new_n - 1;
            }
            if(map_points.count(mid3) == 0) {
                new_n ++;
                map_points[mid3] = new_n - 1;
            }
            
            long int map_p1 = map_points[p1];
            long int map_p2 = map_points[p2];
            long int map_p3 = map_points[p3];
            
            long int map_mid1 = map_points[mid1];
            long int map_mid2 = map_points[mid2];
            long int map_mid3 = map_points[mid3];
            
            bool b1 = (bound_point[map_p1] == true);
            bool b2 = (bound_point[map_p2] == true);
            bool b3 = (bound_point[map_p3] == true);
            
            bound_point[map_mid1] = b2 && b3;
            bound_point[map_mid2] = b3 && b1;
            bound_point[map_mid3] = b1 && b2;
            
            new_m++;
            Triangle new_tri = {p2, mid1, p3};
            new_tris.push_back(new_tri);
            map_tris[new_m - 1][1] = map_p2;
            map_tris[new_m - 1][2] = map_mid1;
            map_tris[new_m - 1][3] = map_p3;
            
            new_m++;
            new_tri = {p3, mid2, p1};
            new_tris.push_back(new_tri);
            map_tris[new_m - 1][1] = map_p3;
            map_tris[new_m - 1][2] = map_mid2;
            map_tris[new_m - 1][3] = map_p1;
            
            new_m++;
            new_tri = {p1, mid3, p2};
            new_tris.push_back(new_tri);
            map_tris[new_m - 1][1] = map_p1;
            map_tris[new_m - 1][2] = map_mid3;
            map_tris[new_m - 1][3] = map_p2;
            
            new_m++;
            new_tri = {mid1, mid2, mid3};
            new_tris.push_back(new_tri);
            map_tris[new_m - 1][1] = map_mid1;
            map_tris[new_m - 1][2] = map_mid2;
            map_tris[new_m - 1][3] = map_mid3;
            
        }
        tris = new_tris;
    }

#endif /* Mesh_h */
