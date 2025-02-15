#include "function.h"

//====================================================
//              Additional function  
//====================================================

struct cmpByCellArea{
    inline bool operator()(Cell*& a, Cell*& b) const {
        double a_area = a->w * a->h;
        double b_area = b->w * b->h;
        return a_area > b_area;
    }
};

struct cmpByCellX{
    inline bool operator()(Cell*& a, Cell*& b) const {
        return a->x < b->x;
    }
};

void findRowsOn(Cell *cell_ptr, std::vector<Row> &row_vec, std::vector<int> &y_rows){
    double row_h = row_vec[0].site_h;
    int row_y_idx = (cell_ptr->y - row_vec[0].start_y)/row_h;//find where the start of c2p belong
    y_rows.clear();

    //do while row index still intersects c2p
    while((row_y_idx * row_h) + row_vec[0].start_y < cell_ptr->getUpRight().y){
        y_rows.push_back(row_y_idx);
        row_y_idx++;
    }

}


bool inBound(Cell *cell_ptr, Window &copied_window){
    if(cell_ptr->getUpRight().x > copied_window.end_x || cell_ptr->x < copied_window.start_x) 
        return false;
    if(cell_ptr->getUpRight().y > copied_window.end_y || cell_ptr->y < copied_window.start_y) 
        return false;
    return true;
}

//====================================================
//                 Main flow
//====================================================

void readInput(std::ifstream &input, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map){
    std::string type, dump;
    while(input >> type){
        if(type[0] == 'F'){//FF
            Cell ff_temp;
            ff_temp.name = type;
            ff_temp.fixed = false;
            input >> ff_temp.x >> ff_temp.y >> ff_temp.w >> ff_temp.h >> dump;
            FF_map[ff_temp.name] = ff_temp;
        }
        else if(type[0] == 'C'){//C
            Cell c_temp;
            c_temp.name = type;
            c_temp.fixed = true;
            input >> c_temp.x >> c_temp.y >> c_temp.w >> c_temp.h >> dump;
            C_map[c_temp.name] = c_temp;
        }
        else{
            double start_x, start_y, site_w, site_h, site_nums;
            input >> start_x >> start_y >> site_w >> site_h >> site_nums;
            Row row_tmp(start_x, start_y, site_w, site_h, site_nums);
            row_vec.push_back(row_tmp);
        }
    }
    //printf("finish reading input\n");
}

void initialPlacement(std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map){
    std::vector<Cell*> cell_unplaced;
    for(auto &c : FF_map){
        simplePlace(&c.second, row_vec, cell_unplaced);
    }

    for(auto &c : C_map){
        simplePlace(&c.second, row_vec, cell_unplaced);
    }
    //printf("Finish initial placement\n");
    //printf("%ld unplaced\n", cell_unplaced.size());

}

void legalization(std::ifstream &input, std::ofstream &output, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map){
    std::string dump, tmp;
    std::vector<std::string> cell_to_remove;
    std::vector<Cell*> cell_unplaced;
    int legal_num = 0;
    int erase_num = 0;

    //reading opt
    while(input >> dump){//cell banking:
        Cell new_FF;
        cell_to_remove.clear();
        input >> tmp;
        while(tmp != "-->"){
            cell_to_remove.push_back(tmp);
            input >> tmp;
        }

        for(auto &c2r: cell_to_remove){
            std::vector<int> y_rows;
            Cell *c2r_ptr = &FF_map[c2r];
            findRowsOn(c2r_ptr, row_vec, y_rows);
            for(auto &y : y_rows)
                row_vec[y].removeCell(c2r_ptr);
            FF_map.erase(c2r);
            erase_num++;
        }

        //input new FF
        input >> new_FF.name >> new_FF.x >> new_FF.y >> new_FF.w >> new_FF.h;
        new_FF.fixed = NOTFIXED;
        FF_map[new_FF.name] = new_FF;

        cell_unplaced.push_back(&FF_map[new_FF.name]);
        //simplePlace(&FF_map[new_FF.name], row_vec, cell_unplaced);

        //if(cell_unplaced.size() != 0)
        //    randomPlace(output, row_vec, cell_unplaced, FF_map);
        //else{
        //    output << std::fixed << FF_map[new_FF.name].x << " " << FF_map[new_FF.name].y << "\n";;
        //    output << "0\n";
        //}
        
        //output
        //if(FF_map[new_FF.name].placed){
        //}

        legal_num++;
        if(legal_num % 100 == 0)
            printf("Finished legalizing %7d cells\n", legal_num);

    }


    printf("%ld unplaced\n", cell_unplaced.size());
    if(cell_unplaced.size() != 0){
        printf("try brute force..\n");
        brutePlace(output, row_vec, FF_map, cell_unplaced);
    }

    printf("%ld unplaced\n", cell_unplaced.size());
    if(cell_unplaced.size() != 0){
        printf("damn..\n");
        realRandomPlace(output, row_vec, cell_unplaced, FF_map);
    }

    printf("Finished legalizing!!\n");
}

void tetrisLegalization(std::ifstream &input, std::ofstream &output, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map){
    Window copied_window; 
    std::string dump, tmp;
    std::vector<std::string> output_strings;
    std::vector<std::string> cell_to_remove;
    std::vector<Cell*> cell_unplaced;
    std::unordered_map<std::string, Cell> cell_in_window;
    int legal_num = 0;

    //reading opt
    while(input >> dump){//cell banking:
        Cell new_FF;
        cell_to_remove.clear();
        input >> tmp;
        while(tmp != "-->"){
            cell_to_remove.push_back(tmp);
            input >> tmp;
        }
        

        for(auto &c2r: cell_to_remove){
            std::vector<int> y_rows;
            Cell *c2r_ptr = &FF_map[c2r];
            findRowsOn(c2r_ptr, row_vec, y_rows);
            for(auto &y : y_rows)
                row_vec[y].removeCell(c2r_ptr);
            FF_map.erase(c2r);
        }

        //input new FF
        input >> new_FF.name >> new_FF.x >> new_FF.y >> new_FF.w >> new_FF.h;
        new_FF.fixed = NOTFIXED;
        FF_map[new_FF.name] = new_FF;
        Cell *cell_ptr, new_FF_for_tetris;
        new_FF_for_tetris = new_FF;

        //for random spiral
        double original_x, original_y, shift_x = 0, shift_y = 0;
        int up_down, left_right;//0: up/left, 1: don't move, 2: down/right
        int tried_times = 0;
        int x_size = 3, y_size = 2;

        bool place_success = false;
        while(place_success == false){
            cell_in_window.clear();
            copied_window.clear();
            cell_unplaced.clear();

            cell_in_window[new_FF.name] = new_FF_for_tetris;
            Cell *cell_ptr = &cell_in_window[new_FF.name];

            copyWindow(cell_ptr, copied_window, cell_in_window, row_vec, x_size, y_size);

            for(auto &c2r: cell_in_window){
                if(c2r.second.name == cell_ptr->name)//skip this block
                    continue;
                std::vector<int> y_rows;
                Cell *c2r_ptr = &c2r.second;
                findRowsOn(c2r_ptr, copied_window.window_rows, y_rows);
                for(auto &y : y_rows){
                    copied_window.window_rows[y].removeCell(c2r_ptr);
                }
                cell_unplaced.push_back(c2r_ptr);
            }
            cell_unplaced.push_back(cell_ptr);

            //set all y of cell to the bottom of window
            for(auto &c: cell_unplaced){
                c->y = copied_window.window_rows[0].start_y;
                c->placed = false;
            }

            tetrisPlace(output, copied_window.window_rows, cell_unplaced, cell_in_window);


            if(cell_unplaced.size() == 0){
                place_success = true;
            }
            //this is window expand
            //else{
            //    x_size += 1;
            //    y_size += 1;
            //    printf("try x: %d, y:%d\n", x_size, y_size);
            //}
            else{
                new_FF_for_tetris.x = row_vec[0].start_x + (rand()%(row_vec[0].site_nums-7) + 3) * row_vec[0].site_w;
                new_FF_for_tetris.y = row_vec[0].start_y + (rand()%(row_vec.size()-7) + 3) * row_vec[0].site_h;
            }
            //this is random place + shift window
            //else{
            //    tried_times++;
            //    double prob = (double)rand()/(RAND_MAX) ;
            //    if(prob > 1./tried_times){//the probility of random will be higher
            //        new_FF_for_tetris.x = row_vec[0].start_x + (rand()%(row_vec[0].site_nums-7) + 3) * row_vec[0].site_w;
            //        new_FF_for_tetris.y = row_vec[0].start_y + (rand()%(row_vec.size()-7) + 3) * row_vec[0].site_h;
            //    }
            //    else{
            //        shift_x += new_FF_for_tetris.w;
            //        shift_y += new_FF_for_tetris.h;
            //        up_down = rand() % 3;
            //        left_right = rand() % 3;
            //        while(up_down == 1 && left_right == 1){//make sure don't stop at same point
            //            up_down = rand() % 3;
            //            left_right = rand() % 3;
            //        }
            //        switch(up_down){
            //            case 0:
            //                new_FF_for_tetris.y = original_y + shift_y;
            //                break;
            //            case 1:
            //                new_FF_for_tetris.y = original_y;
            //                break;
            //            case 2:
            //                new_FF_for_tetris.y = original_y - shift_y;
            //                break;
            //            default:
            //                new_FF_for_tetris.y = original_y;
            //                break;
            //        }
            //        switch(left_right){
            //            case 0:
            //                new_FF_for_tetris.x = original_x - shift_x;
            //                break;
            //            case 1:
            //                new_FF_for_tetris.x = original_x;
            //                break;
            //            case 2:
            //                new_FF_for_tetris.x = original_x + shift_x;
            //                break;
            //            default:
            //                new_FF_for_tetris.x = original_x;
            //                break;
            //        }
            //    }
            //    //tried_times++;
            //    //if(tried_times > 500){
            //    //    x_size++, y_size++;
            //    //    tried_times = 0;
            //    //}
            //    //printf("try new place at (%.0lf, %.0lf)\n", new_FF_for_tetris.x, new_FF_for_tetris.y);
            //}
            //else{
            //    shift_x += new_FF_for_tetris.w;
            //    shift_y += new_FF_for_tetris.h;
            //    up_down = rand() % 3;
            //    left_right = rand() % 3;
            //    while(up_down == 1 && left_right == 1){//make sure don't stop at same point
            //        up_down = rand() % 3;
            //        left_right = rand() % 3;
            //    }
            //    switch(up_down){
            //        case 0:
            //            new_FF_for_tetris.y = original_y + shift_y;
            //            break;
            //        case 1:
            //            new_FF_for_tetris.y = original_y;
            //            break;
            //        case 2:
            //            new_FF_for_tetris.y = original_y - shift_y;
            //            break;
            //        default:
            //            new_FF_for_tetris.y = original_y;
            //            break;
            //    }
            //    switch(left_right){
            //        case 0:
            //            new_FF_for_tetris.x = original_x - shift_x;
            //            break;
            //        case 1:
            //            new_FF_for_tetris.x = original_x;
            //            break;
            //        case 2:
            //            new_FF_for_tetris.x = original_x + shift_x;
            //            break;
            //        default:
            //            new_FF_for_tetris.x = original_x;
            //            break;
            //    }

            //    //expand window and do again
            //    //if(shift_x > row_vec[0].site_w * row_vec[0].site_nums || shift_y > row_vec[0].site_h * (row_vec.size()-1)){
            //    //    x_size++, y_size++;
            //    //    shift_x = 0, shift_y = 0;
            //    //}
            //    //printf("try new place at (%.0lf, %.0lf)\n", new_FF_for_tetris.x, new_FF_for_tetris.y);
            //}
        }

        //remove the cells in window
        for(auto &cinw: cell_in_window){
            std::vector<int> y_rows;
            Cell *c2r_ptr = &FF_map[cinw.first];
            if(cinw.first != new_FF.name){
                findRowsOn(c2r_ptr, row_vec, y_rows);
                for(auto &y : y_rows)
                    row_vec[y].removeCell(c2r_ptr);
            }

            c2r_ptr->x = cell_in_window[c2r_ptr->name].x;
            c2r_ptr->y = cell_in_window[c2r_ptr->name].y;
            c2r_ptr->placed = false;
        }

        //insert the cells in the new placement
        output_strings.clear();
        for(auto &cinw: cell_in_window){
            std::vector<int> y_rows;
            std::string ff_moved_str;
            std::ostringstream strs;
            Cell *c2r_ptr = &FF_map[cinw.first];
            findRowsOn(c2r_ptr, row_vec, y_rows);
            for(auto &y : y_rows)
                row_vec[y].insertCell(c2r_ptr);
            c2r_ptr->placed = true;

            if(cinw.first != new_FF.name){
                strs << std::fixed << c2r_ptr->x << " " << c2r_ptr->y;
                ff_moved_str = c2r_ptr->name + " " + strs.str();
                output_strings.push_back(ff_moved_str);
            }
        }


        output << std::fixed << FF_map[new_FF.name].x << " " << FF_map[new_FF.name].y << "\n";;
        output << output_strings.size() << "\n";
        for(auto &s: output_strings){
            output << std::fixed << s << "\n";
        }
        
        //output
        //if(FF_map[new_FF.name].placed){
        //}

        legal_num++;
        //if(legal_num % 100 == 0)
            //printf("Finished legalizing %7d cells\n", legal_num);


    }

}

//====================================================
//                Placing method 
//====================================================


void simplePlace(Cell *cell_ptr, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced){
    std::vector<int> y_rows;
    double row_h = row_vec[0].site_h;
    int row_y_idx = (cell_ptr->y - row_vec[0].start_y)/row_h;//find where the start of c2p belong
    bool legal = true;
    double legal_x, legal_y;
    cell_ptr->placed = false;


    //if original placement out of bound
    while(cell_ptr->getUpRight().y > row_vec.back().start_y + row_vec.back().site_h){
        cell_ptr->y -= row_h;
    }

    while(cell_ptr->y < row_vec[0].start_y){
        cell_ptr->y += row_h;
    }

    row_y_idx = (cell_ptr->y - row_vec[0].start_y)/row_h;

    //do while row index still intersects c2p
    while((row_y_idx * row_h) + row_vec[0].start_y < cell_ptr->getUpRight().y){
        if(!row_vec[row_y_idx].positionLegal(cell_ptr))
            legal = false;
        y_rows.push_back(row_y_idx);
        row_y_idx++;
    }


    bool found;
    if(!legal){
        
        Segment place_candidate;
        found = false;
        for(auto &seg: row_vec[y_rows[0]].segment_vec){
            if(seg.getLength() >= cell_ptr->w && seg.status == EMPTY){
                place_candidate = seg;
                for(auto &i : y_rows){
                    found = false;
                    for(auto &s: row_vec[i].segment_vec){
                        double inter_start_x, inter_end_x;//find the intersected segment
                        inter_start_x = (s.start_x > place_candidate.start_x) ? s.start_x : place_candidate.start_x;
                        inter_end_x = (s.end_x < place_candidate.end_x) ? s.end_x : place_candidate.end_x;
                        if((inter_end_x - inter_start_x > cell_ptr->w) && s.status == EMPTY){
                            //s.print();
                            place_candidate.start_x = inter_start_x;
                            place_candidate.end_x = inter_end_x;
                            found = true;
                            break;
                        }
                    }
                    if(place_candidate.getLength() < cell_ptr->w || found == false){//segment is too small or didn't found
                        found = false;
                        break;
                    }
                }
            }
            if(found == true){
                cell_ptr->x = place_candidate.start_x;
                for(auto &r: y_rows)
                    row_vec[r].insertCell(cell_ptr);
                cell_ptr->placed = true;
                return;
            }
        }
        //printf("%s unplaced\n", cell_ptr->name.c_str()); 
        cell_unplaced.push_back(cell_ptr);
    }
    else{
        for(auto &r: y_rows)
            row_vec[r].insertCell(cell_ptr);
        cell_ptr->placed = true;
        return;
    }
}



void brutePlace(std::ofstream &output, std::vector<Row> &row_vec, std::unordered_map<std::string, Cell> &FF_map, std::vector<Cell*> &cell_unplaced){
    //first remove all the cells
    int i=0;
    for(auto &c2r: FF_map){
        Cell *c2r_ptr = &c2r.second;
        if(c2r_ptr->placed){
            std::vector<int> y_rows;
            findRowsOn(c2r_ptr, row_vec, y_rows);
            for(auto &y : y_rows)
                row_vec[y].removeCell(c2r_ptr);
            c2r_ptr->placed = false;
            cell_unplaced.push_back(c2r_ptr);
        }
    }
    std::sort(cell_unplaced.begin(), cell_unplaced.end(), cmpByCellArea());

    randomPlace(output, row_vec, cell_unplaced, FF_map);
}

void randomPlace(std::ofstream &output, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced, std::unordered_map<std::string, Cell> &FF_map){
    bool down = true;
    int i = 0;
    std::vector<Cell*> still_unplaced;
    while(!cell_unplaced.empty()){
        Cell *c = cell_unplaced.back();

        cell_unplaced.pop_back();
        if(down){
            if(c->y > row_vec[0].start_y + row_vec[0].site_h)
                c->y -= row_vec[0].site_h;
            else{
                down = false;
            }
        }
        else{
            if(c->getUpRight().y < row_vec.back().start_y){
                c->y += row_vec[0].site_h;
            }
            else{
                still_unplaced.push_back(c);
                down = true;
                continue;
            }
        }


        simplePlace(c, row_vec, cell_unplaced);
        if(c->placed){
            output << std::fixed << c->x << " " << c->y << "\n";;
            output << "0\n";
            printf("placed %s\n", c->name.c_str());
        }
    }
    cell_unplaced = still_unplaced;
}

void realRandomPlace(std::ofstream &output, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced, std::unordered_map<std::string, Cell> &FF_map){
    printf("random these little brats\n");
    while(!cell_unplaced.empty()){
        Cell* c2p = cell_unplaced.back();
        cell_unplaced.pop_back();
        c2p->x = row_vec[0].start_x + (rand()%(row_vec[0].site_nums-4)) * row_vec[0].site_w;
        c2p->y = row_vec[0].start_y + (rand()%(row_vec.size()-4)) * row_vec[0].site_h;

        simplePlace(c2p, row_vec, cell_unplaced);
        if(c2p->placed){
            output << c2p->x << " " << c2p->y << "\n";;
            printf("placed %s\n", c2p->name.c_str());
            printf("%ld still unplaced\n", cell_unplaced.size());
        }
    }
}

void tetrisPlace(std::ofstream &output, std::vector<Row> &row_vec, std::vector<Cell*> &cell_unplaced, std::unordered_map<std::string, Cell> &FF_map_temp){
    int i = 0;
    std::vector<Cell*> still_unplaced;

    while(!cell_unplaced.empty()){
        Cell *c = cell_unplaced.back();
        cell_unplaced.pop_back();

        if(c->getUpRight().y < row_vec.back().start_y){
            c->y += row_vec[0].site_h;
        }
        else{
            still_unplaced.push_back(c);
            continue;
        }


        simplePlace(c, row_vec, cell_unplaced);
        //if(c->placed){
        //    printf("placed %s\n", c->name.c_str());
        //}
    }

    //printf("%ld still unplaced\n", still_unplaced.size());
    cell_unplaced = still_unplaced;
}

//====================================================
//                   Output
//====================================================

void outputDrawLg(std::ofstream &draw_output, double die_lowLx, double die_lowLy, double die_upRx, double die_upRy, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map){
    draw_output << "DieSize " << std::fixed << std::setprecision(0) << die_lowLx << " " << die_lowLy << " " << die_upRx << " " << die_upRy << "\n";
    for(auto &c : FF_map){
        auto f = c.second;
        if(f.placed)
            draw_output << f.name << " " << std::fixed << std::setprecision(0) << f.x << " " << f.y << " " << f.w << " " << f.h << " NOTFIX\n";
    }

    for(auto &c : C_map){
        auto f = c.second;
        if(f.placed)
            draw_output << f.name << " " << std::fixed << std::setprecision(0) << f.x << " " << f.y << " " << f.w << " " << f.h << " FIX\n";
    }
    printf("Finish output .drawlg\n");
}

void outputResult(std::ofstream &result_output, int alpha, int beta, double die_lowLx, double die_lowLy, double die_upRx, double die_upRy, std::unordered_map<std::string, Cell> &FF_map, std::unordered_map<std::string, Cell> &C_map, std::vector<Row> &row_vec){
    result_output << "Alpha " << alpha << "\n";
    result_output << "Beta " << beta << "\n";
    result_output << "DieSize " << std::fixed << std::setprecision(0) << die_lowLx << " " << die_lowLy << " " << die_upRx << " " << die_upRy << "\n";
    for(auto &c : FF_map){
        auto f = c.second;
        if(f.placed)
            result_output << f.name << " " << std::fixed << std::setprecision(0) << f.x << " " << f.y << " " << f.w << " " << f.h << " NOTFIX\n";
    }

    for(auto &c : C_map){
        auto f = c.second;
        if(f.placed)
            result_output << f.name << " " << std::fixed << std::setprecision(0) << f.x << " " << f.y << " " << f.w << " " << f.h << " FIX\n";
    }

    for(auto &r : row_vec){
        result_output << "PlacementRows" << " " << std::fixed << std::setprecision(0) << r.start_x << " " << r.start_y << " " << r.site_w << " " << r.site_h << " " << r.site_nums << "\n";
    }
    printf("Finish output .result\n");

}



//====================================================
//                   Tetris
//====================================================

void copyWindow(Cell *cell_ptr, Window &copied_window, std::unordered_map<std::string, Cell> &FF_map_temp, std::vector<Row> &row_vec, int &x_size, int &y_size){
    //decide the windows x and y by the cell's coordinate
    copied_window.start_x = cell_ptr->x - x_size* cell_ptr->w, copied_window.end_x = cell_ptr->getUpRight().x + x_size* cell_ptr->w;
    copied_window.start_y = cell_ptr->y - y_size* cell_ptr->h, copied_window.end_y = cell_ptr->getUpRight().y + y_size* cell_ptr->h;
    copied_window.w = copied_window.end_x - copied_window.start_x;
    copied_window.h = copied_window.end_y - copied_window.start_y;

    std::vector<int> y_rows;

    //adjust if out of bounds
    while(copied_window.start_y < row_vec[0].start_y){
        copied_window.start_y += cell_ptr->h;
        copied_window.end_y += cell_ptr->h;
        cell_ptr->y += cell_ptr->h;
    }

    while(copied_window.start_x < row_vec[0].start_x){
        copied_window.start_x += cell_ptr->w;
        copied_window.end_x += cell_ptr->w;
        cell_ptr->x += cell_ptr->w;
    }

    while(copied_window.end_y > row_vec.back().start_y){
        copied_window.start_y -= cell_ptr->h;
        copied_window.end_y -= cell_ptr->h;
        cell_ptr->y -= cell_ptr->h;
    }

    while(copied_window.end_x > row_vec[0].start_x + row_vec[0].site_w * row_vec[0].site_nums){
        copied_window.start_x -= cell_ptr->w;
        copied_window.end_x -= cell_ptr->w;
        cell_ptr->x -= cell_ptr->w;
    }


    //find the rows the window is on
    double row_h = row_vec[0].site_h;
    int row_y_idx = (copied_window.start_y - row_vec[0].start_y)/row_h;//find where the start of c2p belong

    //do while row index still intersects window
    while((row_y_idx * row_h) + row_vec[0].start_y < copied_window.end_y){
        y_rows.push_back(row_y_idx);
        row_y_idx++;
    }
    copied_window.rows_in_die = y_rows;



    Segment tmp, window_seg;
    int site_nums = copied_window.w/row_vec[0].site_w;
    window_seg.start_x = copied_window.start_x;
    window_seg.end_x = copied_window.end_x;

    for(auto &r: y_rows){
        std::vector<Segment> seg_vec_tmp;
        Row tmp_row(copied_window.start_x, row_vec[r].start_y, row_vec[0].site_w, row_vec[0].site_h, site_nums);

        for(auto s : row_vec[r].segment_vec){
            if(intersect(s, window_seg)){
                if(s.status == NOTFIXED){
                    if(s.cell_ptr->fixed == 0 && inBound(s.cell_ptr, copied_window)){
                        //s.cell_ptr->print();
                        FF_map_temp[s.cell_ptr->name] = *s.cell_ptr;
                        s.cell_ptr = &FF_map_temp[s.cell_ptr->name];//point to the cell in FF map temp
                    }
                    else{
                        s.status = FIXED;
                    }
                }
                seg_vec_tmp.push_back(s);
            }
        }

        seg_vec_tmp[0].start_x = copied_window.start_x;
        seg_vec_tmp.back().end_x = copied_window.end_x;
        tmp_row.segment_vec = seg_vec_tmp;
        copied_window.window_rows.push_back(tmp_row);
    }

}
