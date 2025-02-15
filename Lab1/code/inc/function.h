#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <map>
#include "tile.h"


using namespace std;


Tile *findPoint(Point aim, map<int, Tile*> &tile_map);
bool inTile(Point aim, Tile *tile_now);
bool inTileHorizontal(Point aim, Tile *tile_now);
bool inTileVertical(Point aim, Tile *tile_now);
void splitHorizontal(Point p, map<int, Tile*> &tile_map, int &space_idx, int outline_h);
void splitVertical(Tile *split_tile, Point p_left, Point p_right, map<int, Tile*> &tile_map, int &space_idx);
void splitVerticalNoLeft(Tile *split_tile, Point p_right, map<int, Tile*> &tile_map, int &space_idx);
void splitVerticalNoRight(Tile *split_tile, Point p_left, map<int, Tile*> &tile_map, int &space_idx);
void merge(Tile *new_tile, Tile *old_tile, map<int, Tile*> &tile_map);
Tile *createTile(Point ori, int w, int h, map<int, Tile*> &tile_map, int &space_idx, int new_index, int outline_w, int outline_h);
void outputLayout(ofstream &output, map<int, Tile*> &tile_map, int outline_w, int outline_h);

#endif