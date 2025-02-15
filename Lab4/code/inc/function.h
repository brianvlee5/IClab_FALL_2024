#ifndef _FUNCTION_H
#define _FUNCTION_H

#define INITIAL_COST -1

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "container.h"


void readInput(std::ifstream &gmp_input, std::ifstream &gcl_input, std::ifstream &cst_input, GridMap &grid_map, std::vector<std::vector<GCell>> &gcell_vec);
void initialRoute(Point &source, Point &target, std::vector<std::vector<GCell>> &gcell_vec, GridMap &grid_map, std::vector<Point> &routing_path);
void aStarRoute(Point &source, Point &target, std::vector<std::vector<GCell>> &gcell_vec, GridMap &grid_map, std::vector<Point> &routing_path);
void outputDrawLg(std::ofstream &draw_output, GridMap &grid_map, std::vector<std::vector<Point>> &routing_paths);
void outputResult(std::ofstream &lg_output, std::vector<std::vector<Point>> &routing_paths, GridMap &grid_map);
void insertCell(std::vector<GCell*> &open_list, GCell *cell_now, double &alpha, double &beta, double &gamma, double &delta);
void eraseCell(std::vector<GCell*> &open_list, GCell *cell_now);

#endif