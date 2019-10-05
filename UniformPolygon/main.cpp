//
//  main.cpp
//  UniformPolygon
//
//  Created by David Kapatsa on 4/2/19.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//
#include <iostream>
#include <fstream>
#include "Polygon.hpp"
#include <random>
#include <ctime>
using namespace std;



int main(int argc, const char * argv[]) {
    
    std::mt19937 gen;
    std::normal_distribution<> d{0,1};
    std::uniform_real_distribution<> u{0,1};
    unsigned int begin = clock();
    gen.seed(begin);

    int noOfPoints = 10000; //30, 1,000,000
    Point * randomPoints;
    randomPoints = new Point[noOfPoints];
    
    //output of all normally distibuted points for the generation
    ofstream file1;
    file1.open ("randomPoints.txt");
    file1 << "x,y\n";
    for (int i = 0; i < noOfPoints; ++i) {
        randomPoints[i].x = d(gen);
        randomPoints[i].y = d(gen);
        file1 << randomPoints[i].x  << "," << randomPoints[i].y << "\n";
    }
    file1.close();

    int howManyPointsInsidePoly = 5000;
    ConvexHull poly(randomPoints, noOfPoints);
    ofstream file2;
    file2.open ("convexHull.txt");
    file2 << "x,y\n";
    std::cout << poly.noOfVertices << "  ";
    for (int i = 0; i < poly.noOfVertices; ++i) {
        file2 << poly.vertices[i].x << "," << poly.vertices[i].y << "\n";
    }
    file2.close();
    

    Point temp;
    ofstream file3;
    file3.open ("uniformOnPolygon.txt");
    file3 << "x,y\n";
    for (int i = 0; i < howManyPointsInsidePoly; ++i){
        temp = poly.uniformInside(gen, u);
        file3 << temp.x << "," << temp.y << "\n";
    }
    
    file3.close();
    
  
    
    
    return 0;
}
