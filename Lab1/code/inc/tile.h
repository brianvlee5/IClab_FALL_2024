#ifndef _TILE_H_
#define _TILE_H_

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstddef>

using namespace std;

class Point{
public:
    int x, y;   

    Point();
    Point(int in_x, int in_y);
    ~Point();
    void print();
    void printFile(ofstream &output);
};

class Tile{
public:
    Tile *rt, *tr, *lb, *bl;
    Point origin;
    int h = 0, w = 0;
    int index = 0;
    bool solid = 0;

    Tile();
    Tile(int in_index, int in_x, int in_y, int in_w, int in_h);
    ~Tile();
    Point getUpRight();
    void print();
    void countAdjacent();
    void countAdjacent(ofstream &output);
};

#endif
