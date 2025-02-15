#ifndef _FUNCTION_H
#define _FUNCTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <cmath>

#include "container.h"

void findRowsOn(Cell *cell_ptr, std::vector<Row> &row_vec, std::vector<int> &y_rows);
bool inBound(Cell *cell_ptr, Window &copied_window);

void readInput(std::ifstream &input, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map);
void initialPlacement(std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map);
void legalization(std::ifstream &input, std::ofstream &output, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map);
void tetrisLegalization(std::ifstream &input, std::ofstream &output, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map);

void simplePlace(Cell *cell_ptr, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced); 
void brutePlace(std::ofstream &output, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map, std::vector<Cell*> &cell_unplaced);
void randomPlace(std::ofstream &output, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced, std::unordered_map<std::string, Cell> &FF_map);
void realRandomPlace(std::ofstream &output, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced, std::unordered_map<std::string, Cell> &FF_map);
void tetrisPlace(std::ofstream &output, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced, std::unordered_map<std::string, Cell> &FF_map_temp);

void outputDrawLg(std::ofstream &draw_output, double die_lowLx, double die_lowLy, double die_upRx, double die_upRy, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map);
void outputResult(std::ofstream &draw_output, int alpha, int beta, double die_lowLx, double die_lowLy, double die_upRx, double die_upRy, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map, std::vector<Row> &row_vec);

void copyWindow(Cell *cell_ptr, Window &copied_window, std::unordered_map<std::string, Cell> &FF_map_temp, std::vector<Row> &row_vec, int &x_size, int &y_size);


#endif