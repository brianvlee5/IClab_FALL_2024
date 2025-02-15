#ifndef _CONTAINER_H
#define _CONTAINER_H

#include <stdio.h>
#include <string>
#include <vector>

class Point{
public:
    int x, y;
    Point();
};

class Block{
public:
    Block *right_child, *left_child, *parent;
    Point origin;
    int w, h;
    std::string name;
    Block();

    Point getUpRight();
    void print();
    void printAdjacent();
};

class Net{
public:
    std::vector<std::string> pin_list;
    int degree;
    
    void print();
};

#endif