#include "function.h"

struct cmpByCellCost{
    inline bool operator()(GCell*& a, GCell*& b) const {
        return a->getCost() > b->getCost();
    }
};

struct cmpByCellCost2{
    inline bool operator()(GCell*& a, GCell*& b) const {
        return a->h_cost > b->h_cost;
    }
};

void readInput(std::ifstream &gmp_input, std::ifstream &gcl_input, std::ifstream &cst_input, GridMap &grid_map, std::vector<std::vector<GCell>> &gcell_vec){
    std::string dump;
    int idx;
    //gmp file
    gmp_input >> dump;//.ra
    gmp_input >> grid_map.routing_area_origin.x >> grid_map.routing_area_origin.y >> grid_map.routing_area_w >> grid_map.routing_area_h;
    gmp_input >> dump;//.g
    gmp_input >> grid_map.grid_w >> grid_map.grid_h;
    gmp_input >> dump;//.c
    gmp_input >> grid_map.chip1_origin.x >> grid_map.chip1_origin.y >> grid_map.chip1_w >> grid_map.chip1_h;
    grid_map.chip1_origin.x += grid_map.routing_area_origin.x;
    grid_map.chip1_origin.y += grid_map.routing_area_origin.y;

    gmp_input >> dump;//.b
    while(gmp_input >> dump, dump != ".c"){
        Point tmp_p;
        gmp_input >> tmp_p.x >> tmp_p.y;
        tmp_p.x += grid_map.chip1_origin.x;
        tmp_p.y += grid_map.chip1_origin.y;
        grid_map.bump_vec1.push_back(tmp_p);
    }

    gmp_input >> grid_map.chip2_origin.x >> grid_map.chip2_origin.y >> grid_map.chip2_w >> grid_map.chip2_h;
    grid_map.chip2_origin.x += grid_map.routing_area_origin.x;
    grid_map.chip2_origin.y += grid_map.routing_area_origin.y;
    gmp_input >> dump;//.b
    while(gmp_input >> idx){
        Point tmp_p;
        gmp_input >> tmp_p.x >> tmp_p.y;
        tmp_p.x += grid_map.chip2_origin.x;
        tmp_p.y += grid_map.chip2_origin.y;
        grid_map.bump_vec2.push_back(tmp_p);
    }

    //gcl file
    gcl_input >> dump;//.ec

    int cell_num_h = grid_map.routing_area_w/grid_map.grid_w, cell_num_v = grid_map.routing_area_h/grid_map.grid_h;
    for(int i=0; i<cell_num_v; i++){
        std::vector<GCell> tmp_vec;
        for(int j=0; j<cell_num_h; j++){
            GCell gcell_tmp;
            gcl_input >> gcell_tmp.le_capacity >> gcell_tmp.be_capacity;
            gcell_tmp.origin.x = j*grid_map.grid_w + grid_map.routing_area_origin.x;
            gcell_tmp.origin.y = i*grid_map.grid_h + grid_map.routing_area_origin.y;
            tmp_vec.push_back(gcell_tmp);
        }
        gcell_vec.push_back(tmp_vec);
    }

    //cst file
    double max_cost = 0;
    cst_input >> dump >> grid_map.alpha;
    cst_input >> dump >> grid_map.beta;
    cst_input >> dump >> grid_map.gamma;
    cst_input >> dump >> grid_map.delta;
    cst_input >> dump;//.v
    cst_input >> grid_map.via_cost;
    cst_input >> dump;//.l
    for(int i=0; i<cell_num_v; i++){
        std::vector<double> tmp_vec;
        for(int j=0; j<cell_num_h; j++){
            double cost;
            cst_input >> cost;
            tmp_vec.push_back(cost);

            if(cost > max_cost)
                max_cost = cost;
        }
        grid_map.cost_map1.push_back(tmp_vec);
    }
    cst_input >> dump;//.l
    for(int i=0; i<cell_num_v; i++){
        std::vector<double> tmp_vec;
        for(int j=0; j<cell_num_h; j++){
            double cost;
            cst_input >> cost;
            tmp_vec.push_back(cost);

            if(cost > max_cost)
                max_cost = cost;
        }
        grid_map.cost_map2.push_back(tmp_vec);
    }
    grid_map.max_cell_cost = max_cost;

    //for(int i=0; i<cell_num_v; i++){
    //    for(int j=0; j<cell_num_h; j++){
    //        printf("%lf ", grid_map.cost_map2[i][j]);
    //    }
    //    printf("\n");
    //}




}

void initialRoute(Point &source, Point &target, std::vector<std::vector<GCell>> &gcell_vec, GridMap &grid_map, std::vector<Point> &routing_path){
    std::vector<GCell*> open_list;
    Point source_index, cell_now_index, target_index;
    GCell* cell_now, *source_ptr, *target_ptr;
    double alpha, beta, gamma, delta;
    alpha = grid_map.alpha;
    beta = grid_map.beta;
    gamma = grid_map.gamma;
    delta = grid_map.delta;

    source_index = grid_map.getIndex(source);  
    target_index = grid_map.getIndex(target);  
    source_ptr = &gcell_vec[source_index.y][source_index.x];
    target_ptr = &gcell_vec[target_index.y][target_index.x];
    source_ptr->layer = M1;
    target_ptr->layer = M1;

    for(int i=0; i<gcell_vec.size(); i++){
        for(int j=0; j<gcell_vec[i].size(); j++){
            gcell_vec[i][j].h_cost = (abs(gcell_vec[i][j].origin.x - target_ptr->origin.x) + abs(gcell_vec[i][j].origin.y - target_ptr->origin.y));
            gcell_vec[i][j].g_cost = INITIAL_COST;//infinity
            gcell_vec[i][j].of_cost = 0;
            gcell_vec[i][j].queued = false;
            gcell_vec[i][j].father = nullptr;
            gcell_vec[i][j].closed = false;
        }
    }


    //these two must be modified if via's on them
    source_ptr->g_cost = grid_map.cost_map1[cell_now_index.y][cell_now_index.x];
    open_list.push_back(source_ptr);
    source_ptr->queued = true;

    while(!open_list.empty()){
        std::sort(open_list.begin(), open_list.end(), cmpByCellCost2());
        cell_now = open_list.back(); 
        open_list.pop_back();
        cell_now_index = grid_map.getIndex(cell_now->origin);
        //cell_now->print();

        if(cell_now == target_ptr){
            //printf("found (%d, %d)\n", target_index.x, target_index.y);
            GCell *f = cell_now;
            f->origin.g_cost = f->g_cost;
            f->origin.h_cost = f->h_cost;
            routing_path.push_back(f->origin);
            while(f->father != nullptr){

                if(f->father->origin.y != f->origin.y)
                    f->be_occupied++;
                else
                    f->le_occupied++;

                f = f->father;
                f->origin.g_cost = f->g_cost;
                f->origin.h_cost = f->h_cost;
                routing_path.push_back(f->origin);
            }
            //printf("\n");
            return;
        }
        //right
        GCell *right, *left, *up, *bottom;
        //printf("%d %d\n", cell_now_index.x+1, grid_map.routing_area_w/grid_map.grid_w);
        if((cell_now_index.x+1) < grid_map.routing_area_w/grid_map.grid_w){
            double tentative_gscore;
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x+1] + cell_now->g_cost + via_wei_cost - grid_map.cost_map1[cell_now_index.y][cell_now_index.x];
            else
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x+1] + cell_now->g_cost;

            right = &gcell_vec[cell_now_index.y][cell_now_index.x+1];

            if(right->closed == false){
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, right->getCost(alpha, beta));
                if(tentative_gscore < right->g_cost || right->g_cost == INITIAL_COST){
                    right->g_cost = tentative_gscore;
                    right->layer = M2;
                    right->father = cell_now;
                }
                if(right->queued == false){
                    open_list.push_back(right);
                    right->queued = true;
                }
            }
        }
        if((cell_now_index.x-1) >= 0){
            double tentative_gscore; 
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);
            double overflow_cost;

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x-1] + cell_now->g_cost + via_wei_cost - grid_map.cost_map1[cell_now_index.y][cell_now_index.x];
            else
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x-1] + cell_now->g_cost;

            left = &gcell_vec[cell_now_index.y][cell_now_index.x-1];


            if(left->closed == false){
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, left->getCost(alpha, beta));
                if(tentative_gscore < left->g_cost || left->g_cost == INITIAL_COST){
                    left->g_cost = tentative_gscore;   
                    left->layer = M2;
                    left->father = cell_now;
                }
                if(left->queued == false){
                    open_list.push_back(left);
                    left->queued = true;
                }
            }
        }
        if((cell_now_index.y+1) < grid_map.routing_area_h/grid_map.grid_h){
            double tentative_gscore; 
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map1[cell_now_index.y+1][cell_now_index.x] + cell_now->g_cost;
            else
                tentative_gscore = grid_map.cost_map1[cell_now_index.y+1][cell_now_index.x] + cell_now->g_cost + via_wei_cost - grid_map.cost_map2[cell_now_index.y][cell_now_index.x];

            up = &gcell_vec[cell_now_index.y+1][cell_now_index.x];


            if(up->closed == false){ 
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, up->getCost(alpha, beta));
                if(tentative_gscore < up->g_cost || up->g_cost == INITIAL_COST){
                    up->g_cost = tentative_gscore;     
                    up->layer = M1;
                    up->father = cell_now;
                }
                if(up->queued == false){
                    open_list.push_back(up);
                    up->queued = true;
                }
            }
        }
        if((cell_now_index.y-1) >= 0){ 
            double tentative_gscore; 
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);
            double overflow_cost;

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map1[cell_now_index.y-1][cell_now_index.x] + cell_now->g_cost;
            else
                tentative_gscore = grid_map.cost_map1[cell_now_index.y-1][cell_now_index.x] + cell_now->g_cost + via_wei_cost - grid_map.cost_map2[cell_now_index.y][cell_now_index.x];

            bottom = &gcell_vec[cell_now_index.y-1][cell_now_index.x];


            if(bottom->closed == false){
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, bottom->getCost(alpha, beta));
                if(tentative_gscore < bottom->g_cost || bottom->g_cost == INITIAL_COST){
                    bottom->g_cost = tentative_gscore;
                    bottom->layer = M1;
                    bottom->father = cell_now;
                }
                if(bottom->queued == false){
                    open_list.push_back(bottom);
                    bottom->queued = true;
                }
            }
        }
        //remove cell from queue
        cell_now->closed = true;
        cell_now->queued = false;
    }
    printf("not found\n");

}

//
void aStarRoute(Point &source, Point &target, std::vector<std::vector<GCell>> &gcell_vec, GridMap &grid_map, std::vector<Point> &routing_path){
    std::vector<GCell*> open_list;
    Point source_index, cell_now_index, target_index;
    GCell* cell_now, *source_ptr, *target_ptr;
    double alpha, beta, gamma, delta;
    alpha = grid_map.alpha;
    beta = grid_map.beta;
    gamma = grid_map.gamma;
    delta = grid_map.delta;

    source_index = grid_map.getIndex(source);  
    target_index = grid_map.getIndex(target);  
    source_ptr = &gcell_vec[source_index.y][source_index.x];
    target_ptr = &gcell_vec[target_index.y][target_index.x];
    source_ptr->layer = M1;
    target_ptr->layer = M1;

    for(int i=0; i<gcell_vec.size(); i++){
        for(int j=0; j<gcell_vec[i].size(); j++){
            gcell_vec[i][j].h_cost = (abs(gcell_vec[i][j].origin.x - target_ptr->origin.x) + abs(gcell_vec[i][j].origin.y - target_ptr->origin.y));
            gcell_vec[i][j].g_cost = INITIAL_COST;//infinity
            gcell_vec[i][j].of_cost = 0;
            gcell_vec[i][j].wire_length = 0;
            gcell_vec[i][j].queued = false;
            gcell_vec[i][j].father = nullptr;
            gcell_vec[i][j].closed = false;
        }
    }


    //these two must be modified if via's on them
    source_ptr->g_cost = grid_map.cost_map1[cell_now_index.y][cell_now_index.x];
    open_list.push_back(source_ptr);
    source_ptr->queued = true;

    while(!open_list.empty()){
        //std::sort(open_list.begin(), open_list.end(), cmpByCellCost());
        //for(int i=0; i<open_list.size(); i++)
        //    printf("%.2lf\n", gamma*open_list[i]->g_cost + alpha*open_list[i]->h_cost + 2*beta*open_list[i]->of_cost);
        //printf("--------\n");
        cell_now = open_list.back(); 
        open_list.pop_back();
        cell_now_index = grid_map.getIndex(cell_now->origin);
        //cell_now->print();

        if(cell_now == target_ptr){
            //printf("found (%d, %d)\n", target_index.x, target_index.y);
            GCell *f = cell_now;
            f->origin.g_cost = f->g_cost;
            f->origin.h_cost = f->h_cost;
            routing_path.push_back(f->origin);
            while(f->father != nullptr){

                if(f->father->origin.y != f->origin.y)
                    f->be_occupied++;
                else
                    f->le_occupied++;

                f = f->father;
                f->origin.g_cost = f->g_cost;
                f->origin.h_cost = f->h_cost;
                f->origin.of_cost = f->of_cost;
                routing_path.push_back(f->origin);
            }
            //printf("\n");
            return;
        }
        //right
        GCell *right, *left, *up, *bottom;
        //printf("%d %d\n", cell_now_index.x+1, grid_map.routing_area_w/grid_map.grid_w);
        if((cell_now_index.x+1) < grid_map.routing_area_w/grid_map.grid_w){
            double tentative_gscore;
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);
            double overflow_cost;

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x+1] + cell_now->g_cost + via_wei_cost;// - grid_map.cost_map1[cell_now_index.y][cell_now_index.x];
            else
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x+1] + cell_now->g_cost;

            right = &gcell_vec[cell_now_index.y][cell_now_index.x+1];

            if(right->le_occupied+1 > right->le_occupied)
                overflow_cost = (right->le_occupied + 1 - right->le_capacity) * 0.5 * grid_map.max_cell_cost;
            else
                overflow_cost = 0;

            if(right->closed == false && right->queued == false){
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, right->getCost(alpha, beta));
                if(gamma * tentative_gscore + alpha * right->wire_length + beta * overflow_cost < right->getCost(alpha, beta, gamma) || right->g_cost == INITIAL_COST){
                //if(tentative_gscore < right->g_cost || right->g_cost == INITIAL_COST){
                    right->of_cost = overflow_cost;
                    right->g_cost = tentative_gscore;
                    right->wire_length = grid_map.grid_w + cell_now->wire_length;
                    right->layer = M2;
                    right->father = cell_now;
                    eraseCell(open_list, right);
                    right->queued = false;
                }
                if(right->queued == false){
                    insertCell(open_list, right, alpha, beta, gamma, delta);
                    //open_list.push_back(right);
                    right->queued = true;
                }
            }
        }
        if((cell_now_index.x-1) >= 0){
            double tentative_gscore; 
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);
            double overflow_cost;

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x-1] + cell_now->g_cost + via_wei_cost;// - grid_map.cost_map1[cell_now_index.y][cell_now_index.x];
            else
                tentative_gscore = grid_map.cost_map2[cell_now_index.y][cell_now_index.x-1] + cell_now->g_cost;

            left = &gcell_vec[cell_now_index.y][cell_now_index.x-1];

            if(cell_now->le_occupied + 1 > cell_now->le_capacity)
                overflow_cost = (cell_now->le_occupied + 1 - cell_now->le_capacity) * 0.5 * grid_map.max_cell_cost;
            else
                overflow_cost = 0;

            if(left->closed == false && left->queued == false){
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, left->getCost(alpha, beta));
                if(gamma * tentative_gscore + beta * overflow_cost + alpha * left->wire_length < left->getCost(alpha, beta, gamma) || left->g_cost == INITIAL_COST){
                //if(tentative_gscore < left->g_cost || left->g_cost == INITIAL_COST){
                    cell_now->of_cost = overflow_cost;
                    left->g_cost = tentative_gscore;   
                    left->wire_length = grid_map.grid_w + cell_now->wire_length;
                    left->layer = M2;
                    left->father = cell_now;
                    eraseCell(open_list, left);
                    left->queued = false;
                }
                if(left->queued == false){
                    insertCell(open_list, left, alpha, beta, gamma, delta);
                    //open_list.push_back(left);
                    left->queued = true;
                }
            }
        }
        if((cell_now_index.y+1) < grid_map.routing_area_h/grid_map.grid_h){
            double tentative_gscore; 
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);
            double overflow_cost;

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map1[cell_now_index.y+1][cell_now_index.x] + cell_now->g_cost;
            else
                tentative_gscore = grid_map.cost_map1[cell_now_index.y+1][cell_now_index.x] + cell_now->g_cost + via_wei_cost;// - grid_map.cost_map2[cell_now_index.y][cell_now_index.x];

            up = &gcell_vec[cell_now_index.y+1][cell_now_index.x];

            if(up->be_occupied+1 > up->be_occupied)
                overflow_cost = (up->be_occupied + 1 - up->be_capacity) * 0.5 * grid_map.max_cell_cost;
            else
                overflow_cost = 0;

            if(up->closed == false && up->queued == false){ 
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, up->getCost(alpha, beta));
                if(gamma * tentative_gscore + beta * overflow_cost + alpha * up->wire_length < up->getCost(alpha, beta, gamma) || up->g_cost == INITIAL_COST){
                //if(tentative_gscore < up->g_cost || up->g_cost == INITIAL_COST){
                    up->of_cost = overflow_cost;
                    up->g_cost = tentative_gscore;     
                    up->wire_length = grid_map.grid_h + cell_now->wire_length;
                    up->layer = M1;
                    up->father = cell_now;
                    eraseCell(open_list, up);
                    up->queued = false;
                }
                if(up->queued == false){
                    insertCell(open_list, up, alpha, beta, gamma, delta);
                    //open_list.push_back(up);
                    up->queued = true;
                }
            }
        }
        if((cell_now_index.y-1) >= 0){ 
            double tentative_gscore; 
            double via_wei_cost = grid_map.delta * grid_map.via_cost + grid_map.gamma * 0.5 * (grid_map.cost_map1[cell_now_index.y][cell_now_index.x] + grid_map.cost_map2[cell_now_index.y][cell_now_index.x]);
            double overflow_cost;

            if(cell_now->layer == M1)
                tentative_gscore = grid_map.cost_map1[cell_now_index.y-1][cell_now_index.x] + cell_now->g_cost;
            else
                tentative_gscore = grid_map.cost_map1[cell_now_index.y-1][cell_now_index.x] + cell_now->g_cost + via_wei_cost;// - grid_map.cost_map2[cell_now_index.y][cell_now_index.x];

            bottom = &gcell_vec[cell_now_index.y-1][cell_now_index.x];

            if(cell_now->be_occupied + 1 > cell_now->be_capacity)
                overflow_cost = (cell_now->be_occupied + 1 - cell_now->be_capacity) * 0.5 * grid_map.max_cell_cost;
            else
                overflow_cost = 0;

            if(bottom->closed == false && bottom->queued == false){
                //printf("%.2lf: %.2lf\n", alpha * tentative_gscore + beta * cell_now->h_cost, bottom->getCost(alpha, beta));
                if(gamma* tentative_gscore + beta * overflow_cost + alpha * bottom->wire_length < bottom->getCost(alpha, beta, gamma) || bottom->g_cost == INITIAL_COST){
                //if(tentative_gscore < bottom->g_cost || bottom->g_cost == INITIAL_COST){
                    cell_now->of_cost = overflow_cost;
                    bottom->wire_length = grid_map.grid_h + cell_now->wire_length;
                    bottom->g_cost = tentative_gscore;
                    bottom->layer = M1;
                    bottom->father = cell_now;
                    eraseCell(open_list, bottom);
                    bottom->queued = false;
                }
                if(bottom->queued == false){
                    insertCell(open_list, bottom, alpha, beta, gamma, delta);
                    //open_list.push_back(bottom);
                    bottom->queued = true;
                }
            }
        }
        //remove cell from queue
        cell_now->closed = true;
        cell_now->queued = false;
    }
    //printf("not found\n");

}

void outputDrawLg(std::ofstream &draw_output, GridMap &grid_map, std::vector<std::vector<Point>> &routing_paths){
    draw_output << ".routing area\n";
    draw_output << grid_map.routing_area_origin.x << " " << grid_map.routing_area_origin.y << std::endl;
    draw_output << grid_map.routing_area_w << " " << grid_map.routing_area_h << std::endl;
    draw_output << grid_map.grid_w << " " << grid_map.grid_h << std::endl;
    draw_output << ".nets\n";
    outputResult(draw_output, routing_paths, grid_map);
}

void outputResult(std::ofstream &lg_output, std::vector<std::vector<Point>> &routing_paths, GridMap &grid_map){
    for(int i=0; i<routing_paths.size(); i++){
        lg_output << "n" << i+1 << "\n";
        Point source = routing_paths[i].back(), target = routing_paths[i].front(), next_cell, this_cell;
        Point wire_start, wire_end;
        std::string layer;
        next_cell = routing_paths[i][routing_paths[i].size()-2];

        if(next_cell.x == source.x + grid_map.grid_w || next_cell.x == source.x - grid_map.grid_w){//right or left
            lg_output << "via\n";
            layer = "M2";
        }
        else{
            layer = "M1";
        }
        wire_start.x = source.x, wire_start.y = source.y;
        lg_output << layer << " " << wire_start.x << " " << wire_start.y;

        for(int j=routing_paths[i].size()-2; j>0; j--){
            this_cell = routing_paths[i][j];
            next_cell = routing_paths[i][j-1];
            if(layer == "M1"){
                if(next_cell.x == this_cell.x + grid_map.grid_w || next_cell.x == this_cell.x - grid_map.grid_w){//right or left
                    wire_end.x = this_cell.x, wire_end.y = this_cell.y;
                    lg_output << " " << wire_end.x << " " << wire_end.y << "\n";
                    lg_output << "via\n";
                    wire_start.x = this_cell.x, wire_start.y = this_cell.y;
                    layer = "M2";
                    lg_output << layer << " " << wire_start.x << " " << wire_start.y;
                }
            }
            else{
                if(next_cell.y == this_cell.y + grid_map.grid_h || next_cell.y == this_cell.y - grid_map.grid_h){//up or bottom
                    wire_end.x = this_cell.x, wire_end.y = this_cell.y;
                    lg_output << " " << wire_end.x << " " << wire_end.y << "\n";
                    wire_start.x = this_cell.x, wire_start.y = this_cell.y;
                    lg_output << "via\n";
                    layer = "M1";
                    lg_output << layer << " " << wire_start.x << " " << wire_start.y;
                }
            }
        }
        lg_output << " " << target.x << " " << target.y << "\n";
        if(layer == "M2"){
            lg_output << "via\n";
        }
        lg_output << ".end\n";
    }
    //printf("finish output result\n");
}

void insertCell(std::vector<GCell*> &open_list, GCell *cell_now, double &alpha, double &beta, double &gamma, double &delta){
    double par_for_h = 0;
    par_for_h = (alpha > par_for_h)  ? alpha : par_for_h;
    par_for_h = (beta  > par_for_h)  ? beta : par_for_h;
    par_for_h = (gamma > par_for_h)  ? gamma : par_for_h;
    par_for_h = (delta > par_for_h)  ? delta : par_for_h;
    par_for_h /= 2;


    double cost_now = beta * cell_now->of_cost + gamma * cell_now->g_cost + alpha * (cell_now->wire_length) + par_for_h * cell_now->h_cost;

    if(!cell_now)
        return;

    for(int i=0; i<open_list.size(); i++){
        double cost_comp;
        GCell *cn = open_list[i];
        cost_comp = beta * cn->of_cost + gamma * cn->g_cost + alpha * (cn->wire_length) + par_for_h * cn->h_cost;
        if(cost_now > cost_comp){
            open_list.insert(open_list.begin() + i, cell_now);
            return;
        }
        else if(cost_now == cost_comp){
            if(cell_now->h_cost < cn->h_cost){
                if(i < open_list.size()){
                    open_list.insert(open_list.begin() + i + 1, cell_now);
                }
                else
                    open_list.push_back(cell_now);
            }
            else{
                open_list.insert(open_list.begin() + i, cell_now);
            }
            return;
        }
    }
    open_list.push_back(cell_now);
}

void eraseCell(std::vector<GCell*> &open_list, GCell *cell_now){
    if(open_list.empty())
        return ;
    for(int i=0; i<open_list.size(); i++){
        if(open_list[i]->origin.x == cell_now->origin.x && open_list[i]->origin.y == cell_now->origin.y){
            open_list.erase(open_list.begin()+i);
            return;
        }
    }
}