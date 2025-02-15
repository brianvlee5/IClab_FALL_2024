#include "container.h"

//====================================================
//                   Point
//====================================================
void Point::print(bool new_line){
    printf("(%d, %d) ", x, y);
    if(new_line)
        printf("\n");
}

void Point::printWithCost(){
    printf("**************************************\n");
    printf("             Point:                   \n");
    printf("         origin: (%d, %d)          \n", x, y);
    printf("        g_cost: %.2lf               \n", g_cost);
    printf("        h_cost: %.2lf               \n", h_cost);
    printf("**************************************\n");
}

//====================================================
//                   Grid Map
//====================================================

Point GridMap::getIndex(Point cell_origin){
    int x_index, y_index;
    Point res;
    x_index = (cell_origin.x - routing_area_origin.x)/grid_w;
    y_index = (cell_origin.y - routing_area_origin.y)/grid_h;

    res.x = x_index, res.y = y_index;
    return res;
}

//====================================================
//                   GCell
//====================================================
double GCell::getCost(){
    return 1.1*g_cost + 1.1*h_cost + 1.5*of_cost;
}

double GCell::getCost(double alpha, double beta, double gamma){
    return gamma * g_cost + beta * of_cost + alpha * wire_length;
}

void GCell::print(){
    printf("**************************************\n");
    printf("             Gcell:                   \n");
    printf("         origin: (%d, %d)          \n", origin.x, origin.y);
    printf("         cost: %.2lf               \n", g_cost+h_cost);
    printf("        g_cost: %.2lf               \n", g_cost);
    printf("        h_cost: %.2lf               \n", h_cost);
    printf("**************************************\n");
}

//====================================================
//                   Net
//====================================================

void Net::print(){
    printf("Net: %d\n", index);
    printf("routing path:\n");
    for(auto &r: routing_path){
        r.print(0);
    }
    printf("\n");
}

double Net::getCost(GridMap &grid_map, std::vector<std::vector<GCell>> &gcell_vec){
    g_cost = 0;
    wire_length = 0;
    of_cost = 0;
    //printf("net %d\n", index);
    for(int i=0; i<routing_path.size(); i++){
        if(i != routing_path.size()-1){
            if(routing_path[i+1].x != routing_path[i].x){
                wire_length += grid_map.grid_w; 
            } 
            else{
                wire_length += grid_map.grid_h; 
            }
        }
        of_cost += routing_path[i].of_cost;
    }
    if(routing_path[routing_path.size()-1].x != routing_path.back().x){
        wire_length += grid_map.grid_w; 
    } 
    else{
        wire_length += grid_map.grid_h; 
    }
    g_cost = routing_path[0].g_cost;

    //printf("g: %.2lf\n", g_cost);
    //printf("wl: %.2lf\n", wire_length);
    return grid_map.gamma * g_cost + grid_map.alpha * wire_length;
}