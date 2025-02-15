#define COOLING_THRESHOLD 0.000001
#define COOLING_COEF 0.99

#include "function.h"
#include "container.h"
#include "bstar.h"

#include <cmath>



int main(int argc, char *argv[]){
    std::unordered_map<std::string, Point> terminal_map;
    std::unordered_map<std::string, Block> block_map, temp_map, best_map;
    std::vector<Net> net_vec;
    int outline_w, outline_h;
    double alpha, alpha_base;
    std::ifstream block_input, net_input;
    std::ofstream layout_output, output;
    clock_t timer, init_time, run_time;

    init_time = clock();

//====================================================
//                   Read Input
//====================================================
    block_input.open(argv[2]);
    net_input.open(argv[3]);
    alpha_base = atof(argv[1]);
    layout_output.open("layout/layout.txt");
    output.open(argv[4]);

    readInputBlock(block_input, block_map, terminal_map, outline_w, outline_h);
    readInputNet(net_input, net_vec);

    alpha = 0.5;
//====================================================
//                 Initialize
//====================================================
    srand(7777);
    BStarTree bst, bst_tmp, bst_best;
    //bst.initialize(block_map, outline_w);

//====================================================
//                 Test
//====================================================
    bst.initializeCompleteBin(block_map);
    //bst.printTreeAPI();

//====================================================
//                 SA
//====================================================

    bool bar_printed = false;
    double temperature = 1;
    double prob;
    int round = 0;
    int n_round = 0, feasable_round = 0;
    int old_cost = bst.getActualCost(alpha, outline_w, outline_h, net_vec, block_map, terminal_map);
    int best_cost = old_cost;//storing the best cost
    int best_actual_cost;
    copyTree(bst, bst_best, block_map, best_map);

    while(temperature > COOLING_THRESHOLD){
        copyTree(bst, bst_tmp, block_map, temp_map);
        //bst.printTreeAPI();
        randomPerturb(bst_tmp, temp_map);
        int new_cost = bst_tmp.getActualCost(alpha, outline_w, outline_h, net_vec, temp_map, terminal_map);
        int delta_cost = old_cost - new_cost;
        double delta_cost_norm = (double)delta_cost/old_cost;

        if(delta_cost > 0){
            old_cost = new_cost;
            copyTree(bst_tmp, bst, temp_map, block_map);
        }
        else{//if the probability is smaller than exp(-delta/t), go for it
            prob = (double)rand()/(RAND_MAX) ;
            if(prob < exp(delta_cost_norm/temperature)){
                old_cost = new_cost;
                copyTree(bst_tmp, bst, temp_map, block_map);
            }
        }
        

        if(new_cost < best_cost && bst_tmp.max_x < outline_w && bst_tmp.max_y < outline_h){
        //if(new_cost < best_cost){
            best_cost = new_cost;
            copyTree(bst_tmp, bst_best, temp_map, best_map);
            bst_best.getActualCost(alpha, outline_w, outline_h, net_vec, best_map, terminal_map);

            int w_violate, h_violate;
            if(bst_best.max_x > outline_w)
                w_violate = bst_best.max_x - outline_w;
            else
                w_violate = 0;

            if(bst_best.max_y > outline_h)
                h_violate = bst_best.max_y - outline_h;
            else
                h_violate = 0;

            //printing
            if(!bar_printed){
                printf("  Time           AREA        DeadSpace(%%)       Cost         Temperature           Violated          alpha     rounds\n");
                printf("---------     ----------    ---------------   ----------     -------------      -----------------   ---------  --------\n");
                bar_printed = true;
            }
            timer = clock();
            printf("%-3.2lf sec      %-10d    %-2.1lf               %-10d       %-.4lf          w:%-5d h:%-5d     %.3lf     %-8d\n", (double)timer/1000000, bst_best.area, 100*(double)(bst_best.area - bst_best.min_area)/bst_best.area, best_cost, temperature, w_violate, h_violate, alpha, round);
        }

        //updating alpha
        if(bst_tmp.max_x < outline_w && bst_tmp.max_y < outline_h)
            feasable_round++;
        n_round++;

        if(bst_best.max_x > outline_w || bst_best.max_y > outline_h)
            alpha = 0.5;
        else{
            if(alpha_base < 0.25)
                alpha = 0.25;
            else if(alpha_base > 0.75)
                alpha = 0.75;
            else
                alpha = alpha_base;
        }
        //updateAlpha(alpha, alpha_base, feasable_round, n_round);

        //if(round % 10000 == 0){
        //    printf("---------------------------------------------\n");
        //    printf("%lf %lf\n", prob, exp(delta_cost_norm/temperature));
        //    printf("round %d\n", round);
        //    printf("temperature: %lf\n", temperature);
        //    printf("(old cost - new cost): %d\n", delta_cost);
        //    printf("---------------------------------------------\n");
        //}
        if(round % 1000 == 0){
            temperature *= COOLING_COEF;
            n_round = 0;
            feasable_round = 0;
        }
        round++;
        timer = clock();
        if(timer-init_time > 295000000)
            break;
    }

    int w_violate, h_violate;
    
    if(bst_best.max_x > outline_w)
        w_violate = bst_best.max_x - outline_w;
    else
        w_violate = 0;

    if(bst_best.max_y > outline_h)
        h_violate = bst_best.max_y - outline_h;
    else
        h_violate = 0;

    best_actual_cost = bst_best.getOutputCost(alpha_base, net_vec, best_map, terminal_map);


    printf("finish sa (%d rounds)\n", round);
    printf("time: %lfs\n", (double)(timer-init_time)/1000000);
    printf("violated: %d %d\n", w_violate, h_violate);
    printf("best cost: %d\n", best_actual_cost);

    outputReport(output, bst_best, best_map, (double)(timer-init_time)/1000000, best_actual_cost);

//====================================================
//                 Finish
//====================================================
    //outputLayout(layout_output, best_map, outline_w, outline_h);
    block_input.close();
    net_input.close();
    layout_output.close();
    output.close();
    return 0;
}