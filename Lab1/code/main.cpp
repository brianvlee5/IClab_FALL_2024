#include <algorithm>
#include <iostream>
#include <fstream>
#include <locale>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <climits>

#include "tile.h"
#include "function.h"

using namespace std;



int main(int argc, char *argv[]){
    //================================variables==================================================
    ifstream input;
    ofstream output, draw_output;
    int outline_w, outline_h;
    int space_idx = -1;//for counting index of space
    string op;
    vector<Point> find_point_res;
    map<int, Tile*> tile_map;
    Tile *initial_space;
    //===========================================================================================

    
    //====================================test===================================================
    //Tile *test_tile[13];
    //test_tile[0] = new Tile(5, 15, 35, 20, 60);
    //test_tile[1] = new Tile(-13, 0, 95, 100, 5);
    //test_tile[2] = new Tile(4, 5, 15, 60, 20);
    //test_tile[3] = new Tile(-1, 0, 15, 5, 20);
    //test_tile[4] = new Tile(-10, 0, 5, 65, 10);
    //test_tile[5] = new Tile(-15, 0, 35, 15, 60);
    //test_tile[6] = new Tile(3, 65, 5, 20, 60);
    //test_tile[7] = new Tile(-6, 85, 5, 15, 60);
    //test_tile[8] = new Tile(-3, 0, 0, 100, 5);//-3 on the testcase
    //test_tile[9] = new Tile(-16, 35, 85, 65, 10);
    //test_tile[10] = new Tile(2, 35, 65, 60, 20);
    //test_tile[11] = new Tile(-9, 95, 65, 5, 20);
    //test_tile[12] = new Tile(1, 35, 35, 30, 30);
    //tile_map.insert(pair<int, Tile*>(test_tile[0]->index, test_tile[0]));
    //tile_map.insert(pair<int, Tile*>(test_tile[1]->index, test_tile[1]));
    //tile_map.insert(pair<int, Tile*>(test_tile[2]->index, test_tile[2]));
    //tile_map.insert(pair<int, Tile*>(test_tile[3]->index, test_tile[3]));
    //tile_map.insert(pair<int, Tile*>(test_tile[4]->index, test_tile[4]));
    //tile_map.insert(pair<int, Tile*>(test_tile[5]->index, test_tile[5]));
    //tile_map.insert(pair<int, Tile*>(test_tile[6]->index, test_tile[6]));
    //tile_map.insert(pair<int, Tile*>(test_tile[7]->index, test_tile[7]));
    //tile_map.insert(pair<int, Tile*>(test_tile[8]->index, test_tile[8]));
    //tile_map.insert(pair<int, Tile*>(test_tile[9]->index, test_tile[9]));
    //tile_map.insert(pair<int, Tile*>(test_tile[10]->index, test_tile[10]));
    //tile_map.insert(pair<int, Tile*>(test_tile[11]->index, test_tile[11]));
    //tile_map.insert(pair<int, Tile*>(test_tile[12]->index, test_tile[12]));
    //tile_map[-3]->rt = tile_map[-6];
    //tile_map[-10]->rt = tile_map[4], tile_map[-10]->tr = tile_map[3], tile_map[-10]->lb = tile_map[-3];
    //tile_map[4]->rt = tile_map[1], tile_map[4]->tr = tile_map[3], tile_map[4]->lb = tile_map[-10], tile_map[4]->bl = tile_map[-1];
    //tile_map[-1]->rt = tile_map[-15], tile_map[-1]->tr = tile_map[4], tile_map[-1]->lb = tile_map[-10];
    //tile_map[3]->rt = tile_map[2], tile_map[3]->tr = tile_map[-6], tile_map[3]->lb = tile_map[-3], tile_map[3]->bl = tile_map[-10];
    //tile_map[-6]->rt = tile_map[-9], tile_map[-6]->lb = tile_map[-3], tile_map[-6]->bl = tile_map[3];
    //tile_map[-15]->rt = tile_map[-13], tile_map[-15]->tr = tile_map[5], tile_map[-15]->lb = tile_map[-1];
    //tile_map[5]->rt = tile_map[-13], tile_map[5]->tr = tile_map[-16], tile_map[5]->lb = tile_map[4], tile_map[5]->bl = tile_map[-15];
    //tile_map[1]->rt = tile_map[2], tile_map[1]->tr = tile_map[3], tile_map[1]->lb = tile_map[4], tile_map[1]->bl = tile_map[5];
    //tile_map[2]->rt = tile_map[-16], tile_map[2]->tr = tile_map[-9], tile_map[2]->lb = tile_map[1], tile_map[2]->bl = tile_map[5];
    //tile_map[-9]->rt = tile_map[-16], tile_map[-9]->lb = tile_map[-6], tile_map[-9]->bl = tile_map[2];
    //tile_map[-13]->lb = tile_map[-15];
    //tile_map[-16]->rt = tile_map[-13], tile_map[-16]->lb = tile_map[2], tile_map[-16]->bl = tile_map[5];
    //tile_map[1]->solid = 1;
    //tile_map[2]->solid = 1;
    //tile_map[3]->solid = 1;
    //tile_map[4]->solid = 1;
    //tile_map[5]->solid = 1;
    
    //input.open(argv[1]);
    //input >> outline_w >> outline_h;
    //initial_space = new Tile(-1, 0, 0, outline_w, outline_h);
    //tile_map.insert(pair<int, Tile*>(initial_space->index, initial_space));


    //createTile(Point(81, 130), 19, 20, tile_map, space_idx, 1, outline_w, outline_h);
    //createTile(Point(24, 122), 9, 13, tile_map, space_idx, 2, outline_w, outline_h);
    //createTile(Point(70, 12), 30, 26, tile_map, space_idx, 3, outline_w, outline_h);
    //createTile(Point(4, 145), 29, 5, tile_map, space_idx, 4, outline_w, outline_h);
    //createTile(Point(0, 12), 70, 110, tile_map, space_idx, 5, outline_w, outline_h);
    //createTile(Point(70, 62), 30, 44, tile_map, space_idx, 6, outline_w, outline_h);
    //splitHorizontal(Point(70, 106), tile_map, space_idx, outline_h);
    //splitHorizontal(Point(70, 62), tile_map, space_idx, outline_h);
    //createTile(Point(65, 5), 20, 60, tile_map, space_idx, 3);
    //createTile(Point(35, 35), 30, 30, tile_map, space_idx, 1);
    //createTile(Point(35, 35), 30, 30, tile_map, space_idx, 1);
 
    //===========================================================================================
     
    //===================================input===================================================

    input.open(argv[1]);
    input >> outline_w >> outline_h;
    initial_space = new Tile(-1, 0, 0, outline_w, outline_h);
    tile_map.insert(pair<int, Tile*>(initial_space->index, initial_space));

    while(input >> op){
        if(op == "P"){
            int aim_x, aim_y; 
            Tile *res;
            input >> aim_x >> aim_y;
            Point aim(aim_x, aim_y);

            //printf("point : %d, %d\n", aim_x, aim_y);
            res = findPoint(aim, tile_map);
            //res->print();
            find_point_res.push_back(res->origin);
        }
        else{
            Tile *new_tile;
            int nt_idx, nt_x, nt_y, nt_w, nt_h;//parameters of new tile(nt)
            Point origin;
            
            nt_idx = stoi(op);
            input >> nt_x >> nt_y >> nt_w >> nt_h;
            origin.x = nt_x, origin.y = nt_y;
            //printf("tile : %d (%d, %d) %d %d\n", nt_idx, nt_x, nt_y, nt_w, nt_h);
            new_tile = createTile(origin, nt_w, nt_h, tile_map, space_idx, nt_idx, outline_w, outline_h);
        }
    }
    //printf("finish reading input\n");

    //===========================================================================================

    //tile_map[301]->print();


    //===================================output===================================================
    output.open(argv[2]);
    //draw_output.open(argv[3]);
    //outputLayout(draw_output, tile_map, outline_w, outline_h);
    //outputLayout(output, tile_map, 100, 100);

    output << tile_map.size() << "\n";

    auto it = tile_map.begin();
    for(int i=1; i<INT_MAX; i++){
        it = tile_map.find(i);
        if(it != tile_map.end())
            break;
    }

    for(;it!=tile_map.end(); it++){
        //printf("%d", (*it).second->index);
        output << (*it).second->index << " ";
        (*it).second->countAdjacent(output);
    }

    for(auto &p : find_point_res){
        p.printFile(output);
    }
    //intf("map: %d\n", tile_map.size());
    //for(auto &m : tile_map){
    //    m.second->print();
    //}
    //===========================================================================================

    return 0;
}