#include "container.h"
#include "function.h"

int main(int argc, char *argv[]){
    std::ifstream gmp_input, gcl_input, cst_input;
    std::ofstream lg_output, draw_output;
    GridMap grid_map;
    std::vector<std::vector<GCell>> gcell_vec;

//====================================================
//                   Input
//====================================================
    gmp_input.open(argv[1]);
    gcl_input.open(argv[2]);
    cst_input.open(argv[3]);
    lg_output.open(argv[4]);
    //draw_output.open(argv[5]);

//====================================================
//                  Test
//====================================================
    std::vector<std::vector<Point>> routing_paths;
    std::vector<Net> net_vec;
    readInput(gmp_input, gcl_input, cst_input, grid_map, gcell_vec);

    //reset gcell edge occupied
    for(int i=0; i<gcell_vec.size(); i++){
        for(int j=0; j<gcell_vec[i].size(); j++){
            gcell_vec[i][j].le_occupied = 0;
            gcell_vec[i][j].be_occupied = 0;
        }
    }

    //for(int i=0; i<grid_map.bump_vec1.size(); i++){
    //    Net tmp_net;
    //    tmp_net.index = i+1;
    //    std::vector<Point> routing_path;
    //    initialRoute(grid_map.bump_vec1[i], grid_map.bump_vec2[i], gcell_vec, grid_map, routing_path);
    //    tmp_net.routing_path = routing_path;
    //    net_vec.push_back(tmp_net);
    //    routing_paths.push_back(routing_path);
    //    printf("finish routing n%d\n", i);
    //}
    //printf("finish initial route\n");

    for(int i=0; i<grid_map.bump_vec1.size(); i++){
        Net tmp_net;
        tmp_net.index = i+1;
        std::vector<Point> routing_path;
        aStarRoute(grid_map.bump_vec1[i], grid_map.bump_vec2[i], gcell_vec, grid_map, routing_path);
        tmp_net.routing_path = routing_path;
        net_vec.push_back(tmp_net);
        routing_paths.push_back(routing_path);
        //printf("finish routing n%d\n", i);
    }

    //double total_cost = 0;
    //for(auto &n: net_vec){
    //    printf("Net %d\n", n.index);
    //    printf("total_cost: %.2lf\n\n", n.getCost(grid_map, gcell_vec));
    //    total_cost += n.getCost(grid_map, gcell_vec);
    //}
    //printf("total cost: %.2lf\n", total_cost);

    outputResult(lg_output, routing_paths, grid_map);

    //outputDrawLg(draw_output, grid_map, routing_paths);

    gmp_input.close();
    gcl_input.close();
    cst_input.close();
    lg_output.close();

    return 0;
}