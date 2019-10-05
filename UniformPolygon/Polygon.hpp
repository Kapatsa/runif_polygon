//
//  Polygon.hpp
//  UniformPolygon
//
//  Created by David Kapatsa on 4/2/19.
//  Copyright © 2019 David Kapatsa. All rights reserved.
//

#ifndef Polygon_hpp
#define Polygon_hpp

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "randoms.hpp"
#include <math.h>
#include <random>
#include <array>
using namespace std;

//std::uniform_real_distribution<> u{0,1};

struct Point{
    double x;
    double y;
    Point(double x = 0, double y = 0): x(x), y(y) {;};

    Point operator - (Point p1){
        Point temp;
        temp.x = x - p1.x;
        temp.y = y - p1.y;
        return temp;
    };
    Point operator + (Point p1){
        Point temp;
        temp.x = x + p1.x;
        temp.y = y + p1.y;
        return temp;
    };
    
    Point operator== (Point p1){
        return (x==p1.x && y==p1.y);
    }
    
    double scalar(Point p1){
        return x*p1.x+y*p1.y;
    };
    
    double norm(){
        return sqrt(x*x+y*y);
    };
    
    void uniform2DPoint(std::mt19937 &gen, std::uniform_real_distribution<> &u){
        //std::uniform_real_distribution<> u{0,1};
        x = u(gen);
        y = u(gen);
    };
    
    //This operator returns the value of cosine of angle between two vectors
    double cosine (Point p1){
        return (x*p1.x + y*p1.y)/(norm()*p1.norm()); //!returns the cosine, not the angle
    };
    
    void print(){
        std::cout << "(" << x << ", " << y << "), " ;
    }
    
};

struct Triangle{
    Point points[3];    //Standard
    Point relPoints[3]; //Triangular coordinates if one of the vertices is moved to (0, 0)
    double areaOfTriangle;
    
    Triangle(){};
    
   Triangle(double x1, double y1, double x2, double y2, double x3, double y3){
        points[0] = Point(x1, y1);
        points[1] = Point(x2, y2);
        points[2] = Point(x3, y3);
        relPoints[0] = Point(0, 0);
        relPoints[1] = points[1] - points[0];
        relPoints[2] = points[2] - points[0];
        areaOfTriangle = area();
    };
    
    Triangle(Point p1, Point p2, Point p3){
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        relPoints[0] = Point(0, 0);
        relPoints[1] = points[1] - points[0];
        relPoints[2] = points[2] - points[0];
        areaOfTriangle = area();
    };
    
    Point uniformInside(std::mt19937 &gen, std::uniform_real_distribution<> &u){
        Point temp{};
        
        //GENERATE RANDOM INSIDE SIMPLE TRIANGLE
        temp.uniform2DPoint(gen, u);
        
        if (temp.x + temp.y > 1) {
            temp.x = 1 - temp.x;
            temp.y = 1 - temp.y;
        };
        
        //COORDINATE CHANGE
        Point tempt{};
        tempt.x = relPoints[2].x * temp.x + relPoints[1].x * temp.y;
        tempt.y = relPoints[2].y * temp.x + relPoints[1].y * temp.y;

        //POINT SHIFT FROM RELATIVE COORDINATES
        tempt.x += points[0].x;
        tempt.y += points[0].y;
        //tempt.print();
        return tempt;
    };
    
    double area(){
        return areaOfTriangle = 0.5 * abs(relPoints[1].x*relPoints[2].y-relPoints[1].y*relPoints[2].x);
    };
    
    //DONE: UNIFORM ON RANDOM TRIANGLE
    //DONE: CALCULATE AREA OF TRIANGLE

};

//TODO: ConvexHull makes a convex polygon
// Class Polygon will make uniform on a polygon
// constructor for Polygon can accept ConvexHull as argument
struct ConvexHull{
    int noOfVertices;
    Point *vertices;
    Triangle *triangles;
    double totalArea = 0;
    std::discrete_distribution<> *dis;
    std::vector <double> weightsAreas;
public:
    //THIS CONSTRUCTOR WILL MAKE A CONVEX POLYGON OUT OF ANY POINTS
    //TODO: IT SHOULD NOT WORK IF STUPID DATA IS INPUT I.E. NO POINTS OR ONE POINT OR POINTS ON THE SAME LINE
    explicit ConvexHull(Point * points, int noOfPoints){
    
        Point simple(1, 0);
        Point previous;
        Point current;
        Point candidate;
        int savedIndex;
        
        
        Point * temp = new Point[noOfPoints];
        
        //find the lowermost point, put it in the Hull
        Point lowermost(0, 10e8);
        for (int i = 0; i < noOfPoints; ++i){
            if (points[i].y < lowermost.y){
                lowermost = points[i];
                savedIndex = i;
            }
        }
        temp[0] = lowermost;
        
        int noOfCandidates = noOfPoints - 1; //lowermost is in the hull already
        std::swap(points[savedIndex], points[noOfCandidates]);
        int noOfVertices = 1;
 
        
        previous = simple;
        current = lowermost;
        double candidateCosine;
        double savedCosine = -1;
        
        while(1){
            for(int i = 0; i < noOfCandidates; ++i){
                candidate = points[i];
                //std::cout << std::endl << i << " candidate is";
                //candidate.print();
                candidateCosine = (current-previous).cosine(candidate-current);
                //std::cout << "cosine: " << candidateCosine << std::endl;
                if (candidateCosine  > savedCosine){
                    savedIndex = i;
                    savedCosine = candidateCosine;
                }
            }
            
            //CHECKING WITH THE LOWERMOST ELEMENT
            if((current-previous).cosine(lowermost-current) > savedCosine){
                --noOfCandidates;
                break;
            }

            --noOfCandidates;
            ++noOfVertices;
            temp[noOfVertices-1] = points[savedIndex];
            previous = current;
            current = points[savedIndex];
           // std::cout << "####I was chosen: ";
            //current.print();
            std::swap(points[savedIndex], points[noOfCandidates]);
            savedCosine = -1;
        }
        
        vertices = new Point[noOfVertices];
        for(int i = 0; i < noOfVertices; ++i){
            vertices[i] = temp[i];
        }
        delete [] temp;
        temp = nullptr;
        
        
        //TRIANGLES (noOfVertices - 2)
        std::cout << std::endl << "Triangles:" <<std::endl;
        triangles = new Triangle [noOfVertices - 2];
        for(int i = 0 ; i < noOfVertices - 2; ++i){
            triangles[i].points[0] = vertices[0];
            std::cout << " ";
            triangles[i].points[0].print();
            triangles[i].points[1] = vertices[i+1];
            triangles[i].points[1].print();
            triangles[i].points[2] = vertices[i+2];
            triangles[i].points[2].print();
            triangles[i].points[0].print();
            std::cout << " ";
            triangles[i].relPoints[0] = Point(0, 0);
            triangles[i].relPoints[1] = triangles[i].points[1] - vertices[0];
            triangles[i].relPoints[2] = triangles[i].points[2] - vertices[0];
            triangles[i].areaOfTriangle = triangles[i].area();
            totalArea += triangles[i].areaOfTriangle;
            
            ofstream file4;
            file4.open ("triangles.txt");
            file4 << "x,y\n";
            for (int i = 0; i < noOfVertices-2; ++i) {
                file4 << triangles[i].points[0].x  << "," << triangles[i].points[0].y << "\n";
                file4 << triangles[i].points[1].x  << "," << triangles[i].points[1].y << "\n";
                file4 << triangles[i].points[2].x  << "," << triangles[i].points[2].y << "\n";
            }
            file4 << triangles[noOfVertices-3].points[0].x  << "," << triangles[noOfVertices-3].points[0].y << "\n";
            file4.close();
        }
        
        std::cout << std::endl << std::endl <<  "Cumulative Areas:" <<std::endl;
        //cumulativeAreas = new double[noOfVertices - 2];
        
        weightsAreas.push_back(triangles[0].areaOfTriangle/totalArea);
        for(int i = 1; i < noOfVertices - 2; ++i){
            weightsAreas.push_back(triangles[i].areaOfTriangle/totalArea);
            std::cout << " " << i << ": " << weightsAreas[i];
        }
        //
        dis =  new std::discrete_distribution<> (weightsAreas.begin(), weightsAreas.end());
        //
        std::cout << std::endl << std::endl <<  "Convex Hull Vertices:" <<std::endl;
        for(int i = 0; i < noOfVertices; ++i){
            vertices[i].print();
        }
        std::cout << "no of vertices: " << noOfVertices << "\n\n";
        
        ofstream file2;
        file2.open ("convexHull.txt");
        file2 << "x,y\n";
        std::cout << noOfVertices;
        for (int i = 0; i < noOfVertices; ++i) {
            file2 << vertices[i].x << "," << vertices[i].y << "\n";
        }
        file2.close();
    }
    
    ~ConvexHull(){
        delete [] vertices;
        delete [] triangles;
        delete dis;
    }
    
    Point uniformInside(std::mt19937 &gen, std::uniform_real_distribution<> &u){
        
        Point temp;
        //Initializing inside every call is terrible, so moved to the definition
        //std::discrete_distribution<> dis(weightsAreas.begin(), weightsAreas.end());
        temp = triangles[dis->operator()(gen)].uniformInside(gen, u);
        
        return temp;
    };
    
};

#endif /* Polygon_hpp */
