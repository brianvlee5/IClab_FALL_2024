#include "container.h"

bool Row::positionLegal(Cell* cell_ptr){
    double right_bound = start_x + site_w * site_nums;
    //out of boundary
    if(cell_ptr->x > right_bound || cell_ptr->getUpRight().x > right_bound)
        return false;

    //intersect with other cell
    for(auto &seg: this->segment_vec){
        if(intersect(seg, cell_ptr->getSegment())){
            if(seg.status != EMPTY)
                return false;
        }
    }
    return true;
}

void Row::insertCell(Cell* cell_ptr){
    std::vector<Segment> new_segs;
    bool placed = false;
    Segment c2p_seg = cell_ptr->getSegment();
    for(int i=0; i<segment_vec.size(); i++){
        if(intersect(segment_vec[i], c2p_seg) && !placed){
            segment_vec[i].end_x = c2p_seg.start_x;
            if(segment_vec[i].getLength() != 0)
                new_segs.push_back(segment_vec[i]);
            new_segs.push_back(c2p_seg);

            Segment tmp;
            if(i < segment_vec.size()-1){
                tmp.start_x = c2p_seg.end_x, tmp.end_x = segment_vec[i+1].start_x;
            }
            else{
                tmp.start_x = c2p_seg.end_x, tmp.end_x = start_x + site_w * site_nums;
            }
            if(tmp.getLength() > 0)
                new_segs.push_back(tmp);
            placed = true;

        }
        else{
            if(segment_vec[i].getLength() > 0)
                new_segs.push_back(segment_vec[i]);
        }
    }
    this->segment_vec = new_segs;
}

void Row::removeCell(Cell* cell_ptr){
    std::vector<Segment> new_segs;
    Segment c2r_seg = cell_ptr->getSegment();
    for(int i=0; i<segment_vec.size(); i++){
        if(intersect(segment_vec[i], c2r_seg)){
            segment_vec[i].status = EMPTY;
            segment_vec[i].cell_ptr = nullptr;
            break;
        }
    }
    for(int i=0; i<segment_vec.size(); i++){
        if(i>0){
            if(new_segs.back().status == EMPTY && segment_vec[i].status == EMPTY){
                new_segs.back().end_x = segment_vec[i].end_x;
                continue;
            }
        }
        new_segs.push_back(segment_vec[i]);
    }
    segment_vec = new_segs;
}

void Row::printSegment(){
    printf("=========== Segment ===========\n");
    for(auto &s: segment_vec){
        s.print();
    }
    printf("===============================\n");
}

void Row::print(){
    printf("**************************************\n");
    printf("                row                  \n");
    printf("      origin: (%.2lf, %.2lf)          \n", start_x, start_y);
    printf("       w: %.2lf h: %.2lf  num: %d     \n", site_w, site_h, site_nums);
    printf("**************************************\n");
}

Row::Row(double s_x, double s_y, double s_w, double s_h, int s_nums){
    Segment tmp;
    start_x = s_x, start_y = s_y, site_w = s_w, site_h = s_h, site_nums = s_nums; 
    tmp.start_x = start_x, tmp.end_x = start_x + site_nums * site_w;
    segment_vec.push_back(tmp);
}


//====================================================
//                    Segment
//====================================================
double Segment::getLength(){
    double res;
    res = end_x - start_x;
    return res;
}

void Segment::print(){
    std::string tmp;
    switch(status){
        case EMPTY:
            tmp = "EMPTY";
            break;
        case FIXED:
            tmp = "C";
            break;
        case NOTFIXED:
            tmp = "FF";
            break;
        default:
            tmp = "WTF";
            break;
    }
    printf("(%5s)[%.2lf, %.2lf]\n", tmp.c_str(), start_x, end_x);
}



//====================================================
//                   Cell
//====================================================

Segment Cell::getSegment(){
    Segment res;
    res.start_x = x;
    res.end_x = x + w;
    res.status = (fixed) ? FIXED : NOTFIXED;
    res.cell_ptr = this;
    return res;
}

Point Cell::getUpRight(){
    Point res;
    res.x = x+w, res.y = y+h;
    return res;
}

void Cell::print(){
    printf("**************************************\n");
    printf("            cell: %s                 \n", name.c_str());
    printf("      origin: (%.2lf, %.2lf)          \n", x, y);
    printf("       w: %.2lf h: %.2lf              \n", w, h);
    printf("**************************************\n");
}

//====================================================
//                    Window
//====================================================
void Window::print(){
    int i=0;
    printf("**************************************\n");
    printf("               Window                 \n");
    printf("        start(%.0lf, %.0lf)         \n", start_x, start_y);
    printf("       w: %.0lf h: %.0lf              \n", w, h);
    printf("       segments:              \n");
    for(auto &r: window_rows){
        printf("------------------row %d------------------\n", rows_in_die[i++]);
        for(auto &a: r.segment_vec){
            a.print();
        }
    }
    printf("**************************************\n");
}

void Window::clear(){
    start_x = 0, start_y = 0, end_x = 0, end_y = 0;
    w = 0, h = 0;
    window_rows.clear();
    rows_in_die.clear();
}


//====================================================
//                   Additional
//====================================================
bool intersect(Segment seg_a, Segment seg_b){
    if(seg_a.start_x < seg_b.end_x && seg_a.end_x > seg_b.start_x)
        return true;
    else
        return false;
}