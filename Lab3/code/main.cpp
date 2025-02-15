#include "container.h"
#include "function.h"



int main(int argc, char *argv[]){
    std::string dump;//for dumping unneeded words
    int alpha, beta, die_lowLx, die_lowLy, die_upRx, die_upRy;
    std::ifstream input;
    std::ofstream output, draw_output, result_output;
    std::vector<Row> row_vec;
    std::unordered_map<std::string, Cell> FF_map;
    std::unordered_map<std::string, Cell> C_map;
    std::map<double, Cell*> cell_list;

//====================================================
//                   Read Input
//====================================================
    input.open(argv[1]);
    input >> dump >> alpha;
    input >> dump >> beta;
    input >> dump >> die_lowLx >> die_lowLy >> die_upRx >> die_upRy;
    readInput(input, row_vec, FF_map, C_map);
    input.close();
    srand(5487);




//====================================================
//                   Test
//====================================================

    /* 
    Cell test_cell, t, lt;
    Row test_row(15300, 16800, 1, 2100, 1269390);
    Row test_row1(15300, 18900, 1, 2100, 1269390);
    Row test_row2(15300, 21000, 1, 2100, 1269390);
    Row test_row3(15300, 23100, 1, 2100, 1269390);
    Row test_row4(15300, 25200, 1, 2100, 1269390);
    std::vector<Row> test_row_vec;
    test_row_vec.push_back(test_row);
    test_row_vec.push_back(test_row1);
    test_row_vec.push_back(test_row2);
    test_row_vec.push_back(test_row3);
    test_row_vec.push_back(test_row4);

    test_cell.x = 415300, test_cell.w = 9690;
    test_cell.y = 1283100, test_cell.h = 2100;
    test_cell.fixed = NOTFIXED;

    lt.x = 20000, lt.w = 1000;
    lt.y = 18900, lt.h = 2100;
    lt.fixed = NOTFIXED;

    t.x = 20200, t.w = 90;
    t.y = 16800, t.h = 2300;
    t.fixed = NOTFIXED;

    //simplePlace(&test_cell, test_row_vec);
    //simplePlace(&lt, test_row_vec);
    //simplePlace(&t, test_row_vec);

    //for(auto &rv : test_row_vec){
    //    rv.print();
    //    rv.printSegment();
    //}

    std::vector<std::vector<Segment>> test_seg_vec;
    std::vector<Cell*> test_cell_vec;
    */
    

    input.open(argv[2]);
    output.open(argv[3]);
    //draw_output.open(argv[4]);//for drawing
    //result_output.open(argv[5]);//for result
    initialPlacement(row_vec, FF_map, C_map);
    tetrisLegalization(input, output, row_vec, FF_map);
    //legalization(input, output, row_vec, FF_map);
    //copyWindow(&test_cell, test_seg_vec, test_cell_vec, row_vec);
    //outputDrawLg(draw_output, die_lowLx, die_lowLy, die_upRx, die_upRy, FF_map, C_map);
    //outputResult(result_output, alpha, beta, die_lowLx, die_lowLy, die_upRx, die_upRy, FF_map, C_map, row_vec);
    //printf("%ld\n", row_vec.size());
    //for(auto &r: row_vec[603].segment_vec)
    //    r.print();
    //for(auto &r: row_vec[602].segment_vec)
    //    r.print();
    //for(auto &r: row_vec[601].segment_vec)
    //    r.print();
    //for(auto &r: row_vec[600].segment_vec)
    //    r.print();
    input.close();
    output.close();
    //draw_output.close();
    //result_output.close();


    return 0;
}
