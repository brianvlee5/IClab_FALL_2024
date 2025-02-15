#ifndef _FUNCTION_H
#define _FUNCTION_H
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <vector>

#include "container.h"
#include "bstar.h"


void readInputBlock(std::ifstream &block_input, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map, int &outline_w, int &outline_h);
void readInputNet(std::ifstream &net_input, std::vector<Net> &net_vec);
void outputLayout(std::ofstream &layout_output, std::unordered_map<std::string, Block> &block_map, int outline_w, int outline_h);
void copyTree(BStarTree &original_tree, BStarTree &dest_tree, std::unordered_map<std::string, Block> &original_map, std::unordered_map<std::string, Block> &dest_map);
void copyTreeTraverse(std::string block_name, std::unordered_map<std::string, Block> &original_map, std::unordered_map<std::string, Block> &dest_map);
void randomPerturb(BStarTree &bst, std::unordered_map<std::string, Block> &block_map);
void outputReport(std::ofstream &output, BStarTree &bst, std::unordered_map<std::string, Block> &block_map, double run_time, int &cost);
void updateAlpha(double &alpha, double &alpha_base, int &feasable_round, int &n_round);


#endif