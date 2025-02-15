#include "tile.h"
#include <cstddef>
#include <ostream>


Point::Point(){

}

Point::Point(int in_x, int in_y){
    x = in_x, y = in_y;
}

Point::~Point(){

}

void Point::print(){
    printf("%d %d\n", x, y); 
}

void Point::printFile(ofstream &output){
    output << x << " " << y << "\n";
}

Point Tile::getUpRight(){
    Point res;
    res.x = origin.x + w, res.y = origin.y + h;
    return res;
}

void Tile::print(){
    printf("block %d\n", index);
    printf("origin:(%d, %d)\n", origin.x, origin.y);
    printf("solid: %d\n", solid);
    printf("w: %d h: %d\n", w, h);
    printf("rt: %d, tr: %d, lb: %d, bl: %d\n", 
    (rt != nullptr) ? rt->index : 0,
    (tr != nullptr) ? tr->index : 0,
    (lb != nullptr) ? lb->index : 0,
    (bl != nullptr) ? bl->index : 0 );
}

void Tile::countAdjacent(ofstream &output){
    int space_n;
    int block_n;
    Tile *cntr;

    space_n = 0, block_n = 0;

    //top
    if(rt != nullptr){
        cntr = rt;
        while(cntr->getUpRight().x > origin.x){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->bl != nullptr)
                cntr = cntr->bl;
            else
                break;
        }
    }

    //right
    if(tr != nullptr){
        cntr = tr;
        while(cntr->getUpRight().y > origin.y){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->lb != nullptr)
                cntr = cntr->lb;
            else
                break;
        }
    }

    //left
    if(bl != nullptr){
        cntr = bl;
        while(cntr->origin.y < this->getUpRight().y){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->rt != nullptr)
                cntr = cntr->rt;
            else
                break;
        }
    }

    //bottom
    if(lb != nullptr){
        cntr = lb;
        while(cntr->origin.x < this->getUpRight().x){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->tr != nullptr)
                cntr = cntr->tr;
            else
                break;
        }
    }
    //printf("space: %d\n", space_n);
    //printf("block: %d\n", block_n);
    //printf(" %d %d\n", block_n, space_n);
    output << block_n << " " << space_n << "\n";

}

void Tile::countAdjacent(){
    int space_n;
    int block_n;
    Tile *cntr;

    space_n = 0, block_n = 0;

    //top
    if(rt != nullptr){
        cntr = rt;
        while(cntr->getUpRight().x > origin.x){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->bl != nullptr)
                cntr = cntr->bl;
            else
                break;
        }
    }

    //right
    if(tr != nullptr){
        cntr = tr;
        while(cntr->getUpRight().y > origin.y){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->lb != nullptr)
                cntr = cntr->lb;
            else
                break;
        }
    }

    //left
    if(bl != nullptr){
        cntr = bl;
        while(cntr->origin.y < this->getUpRight().y){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->rt != nullptr)
                cntr = cntr->rt;
            else
                break;
        }
    }

    //bottom
    if(lb != nullptr){
        cntr = lb;
        while(cntr->origin.x < this->getUpRight().x){
            (cntr->solid) ? block_n++ : space_n++;
            if(cntr->tr != nullptr)
                cntr = cntr->tr;
            else
                break;
        }
    }
    //printf("space: %d\n", space_n);
    //printf("block: %d\n", block_n);
    printf(" %d %d\n", block_n, space_n);
}

Tile::Tile(){
    rt = nullptr, tr = nullptr, lb = nullptr, bl = nullptr;
    
}

Tile::Tile(int in_index, int in_x, int in_y, int in_w, int in_h){
    rt = nullptr, tr = nullptr, lb = nullptr, bl = nullptr;
    origin = Point(in_x, in_y);
    h = in_h, w = in_w;
    index = in_index;
}

Tile::~Tile(){
    rt = nullptr, tr = nullptr, lb = nullptr, bl = nullptr;
}