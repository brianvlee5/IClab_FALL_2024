#ifndef _CONTAINER_H
#define _CONTAINER_H

#define EMPTY 0
#define FIXED 1
#define NOTFIXED 2
#define NEWPLACED 3

#include <string>
#include <map>
#include <vector>


class Point{
public:
    double x = 0, y = 0;
};

class Segment;



class Cell{
public:
    double x = 0, y = 0;//origin x, y
    double w = 0, h = 0;
    bool fixed = false;
    bool placed = false;
    std::string name;

    Point getUpRight();
    Segment getSegment();
    void print();
};

class Segment{
public:
    double start_x = 0, end_x = 0;
    int status = EMPTY;
    Cell *cell_ptr = nullptr;

    double getLength();
    void print();
};


class Row{
public:
    double start_x = 0, start_y = 0;
    double site_w = 0, site_h = 0;
    int site_nums = 0;
    std::vector<Segment> segment_vec;

    Row(){};
    Row(double start_x, double start_y, double site_w, double site_h, int site_nums);

    Cell* findLowerBound(double origin_x);
    bool positionLegal(Cell* cell_ptr);
    void insertCell(Cell* cell_ptr);
    void removeCell(Cell* cell_ptr);

    void printSegment();
    void print();
};

class Window{
public:
    double start_x = 0, start_y = 0, end_x = 0, end_y = 0;
    double w = 0, h = 0;
    std::vector<Row> window_rows;
    std::vector<int> rows_in_die;//the original rows in die

    void print();
    void clear();
};


bool intersect(Segment seg_a, Segment seg_b);


#endif