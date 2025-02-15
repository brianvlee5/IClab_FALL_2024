#ifndef _CONTAINER_H
#define _CONTAINER_H

#define M1 true
#define M2 false

#include <iostream>
#include <stdio.h>
#include <vector>

class Point{
public:
    int x = 0, y = 0;
    double g_cost = 0, h_cost = 0, of_cost = 0;
    void print(bool new_line);
    void printWithCost();
};

class GridMap{
public:
    int routing_area_w, routing_area_h;
    int grid_w, grid_h;
    double alpha, beta, gamma, delta;
    Point routing_area_origin;
    Point chip1_origin, chip2_origin;//1 is source, 2 is target
    int chip1_w, chip1_h, chip2_w, chip2_h;
    double via_cost, max_cell_cost;
    std::vector<Point> bump_vec1, bump_vec2;
    std::vector<std::vector<double>> cost_map1, cost_map2;

    Point getIndex(Point cell_origin);
};

class GCell{
public:
    double g_cost = 0, h_cost = 0, of_cost = 0, wire_length = 0;
    int le_capacity, be_capacity;
    int le_occupied, be_occupied;
    bool closed = false;
    bool queued = false;
    Point origin;
    GCell *father = nullptr;
    bool layer;

    void print();
    double getCost();
    double getCost(double alpha, double beta, double gamma);
};

class Net{
public:
    int index;
    std::vector<Point> routing_path;
    double g_cost = 0;
    double of_cost = 0;
    double wire_length = 0;

    void print();
    double getCost(GridMap &grid_map, std::vector<std::vector<GCell>> &gcell_vec);
};


#endif