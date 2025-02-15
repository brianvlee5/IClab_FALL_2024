#include "container.h"

Point::Point(){
    x=0, y=0;
}

Point Block::getUpRight(){
    Point res;
    res.x = origin.x + w;
    res.y = origin.y + h;
    return res;
}

void Block::print(){
    printf("**************************************\n");
    printf("            block: %s                 \n", name.c_str());
    printf("            origin: (%d, %d)          \n", origin.x, origin.y);
    printf("             w: %d h: %d              \n", w, h);
    printf("**************************************\n");
}

void Block::printAdjacent(){
    printf("**************************************\n");
    printf("            block: %s                 \n", name.c_str());
    if(right_child != nullptr)
        printf("            right: %s          \n", right_child->name.c_str());
    else
        printf("            right: x            \n");
    
    if(left_child != nullptr)
        printf("            left: %s          \n", left_child->name.c_str());
    else
        printf("            left: x            \n");

    if(parent != nullptr)
        printf("            parent: %s          \n", parent->name.c_str());
    else
        printf("            parent: x            \n");

    printf("**************************************\n");
}

Block::Block(){
    left_child = nullptr;
    right_child = nullptr;
    parent = nullptr;
    w = 0, h = 0;
    origin.x = 0, origin.y = 0;
}

void Net::print(){
    printf("**************************************\n");
    printf("           net degree: %d            \n", degree);

    for(int i=0; i<pin_list.size(); i++){
        printf("      pin%3d: %s                    \n", i, pin_list[i].c_str());
    }
    printf("**************************************\n");

}