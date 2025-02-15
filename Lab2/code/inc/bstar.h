#ifndef _BSTAR_H
#define _BSTAR_H
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <string>
#include <queue>

#include "container.h"

class BStarTree{
public:
    Block *root;
    std::vector<Block*> up_contour;
    std::vector<std::string> block_name_vec;
    int max_x, max_y;
    int area;
    int wire_length;
    int min_area;

    void initialize(std::unordered_map<std::string, Block> &block_map, int &outline_w);
    void initializeCompleteBin(std::unordered_map<std::string, Block> &block_map);
    void updateContour(Block *block_placed);
    int getYPlacement(Block *block_to_place);
    int getArea();
    int getWireLength(std::vector<Net> &net_vec, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map);
    int getOutputCost(double &alpha, std::vector<Net> &net_vec, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map);
    int getActualCost(double &alpha, int outline_w, int outline_h, std::vector<Net> &net_vec, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map);

    void reFloorPlan();
    void reFloorPlanTraverse(Block *block_to_place);
    void rotate(Block *aim_block);
    void swapBlock(Block *aim_block1, Block *aim_block2);
    void moveBlock(Block *aim_block, std::unordered_map<std::string, Block> &block_map);
    void deleteBlock(Block *aim_block);

    //for debug
    void printTree(Block *block);
    void printTreeAPI();
    void printUpContour();
};



#endif