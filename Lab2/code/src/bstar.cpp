#include "bstar.h"


void BStarTree::initializeCompleteBin(std::unordered_map<std::string, Block> &block_map){
    min_area = 0;
    //construct block name vec
    for(auto &b : block_map){
        block_name_vec.push_back(b.second.name);
    }

    //start constructing bst
    std::queue<Block*> bfs;
    std::vector<int> inserted_vec(block_name_vec.size(), 0);
    int blocks_left = block_name_vec.size();

    //construct root
    Block *aim_block, *cur;
    int rand_num, map_size = block_map.size();
    aim_block = &block_map[block_name_vec[0]];
    inserted_vec[0] = 1;
    bfs.push(aim_block);
    root = aim_block;
    min_area += aim_block->w * aim_block->h;
    blocks_left--;

    while(blocks_left > 0){
        cur = bfs.front();

        //random left child
        do{
            rand_num = rand() % map_size;
        }while(inserted_vec[rand_num]);
        inserted_vec[rand_num] = 1;
        aim_block = &block_map[block_name_vec[rand_num]];
        aim_block->parent = nullptr, aim_block->right_child = nullptr, aim_block->left_child = nullptr;
        cur->right_child = aim_block, aim_block->parent = cur;
        bfs.push(aim_block);
        min_area += aim_block->w * aim_block->h;
        blocks_left--;
        if(blocks_left <= 0)
            break;

        //random right child
        do{
            rand_num = rand() % map_size;
        }while(inserted_vec[rand_num]);

        inserted_vec[rand_num] = 1;
        aim_block = &block_map[block_name_vec[rand_num]];
        aim_block->parent = nullptr, aim_block->right_child = nullptr, aim_block->left_child = nullptr;
        cur->left_child = aim_block, aim_block->parent = cur;
        bfs.push(aim_block);
        min_area += aim_block->w * aim_block->h;
        blocks_left--;
        bfs.pop();//pop out cur
    }
    printf("finish initialize\n");
    reFloorPlan();
}

void BStarTree::initialize(std::unordered_map<std::string, Block> &block_map, int &outline_w){
    //really initializing things
    max_x = 0, max_y = 0;

    //initializing tree
    root = &(block_map.begin()->second);//begin is a iterator, second is block
    root->origin.x = 0, root->origin.y = 0;
    Block *cur = root, *left_parent = root;
    block_name_vec.push_back(root->name);
    updateContour(root);
    if(root->getUpRight().x > max_x)
        max_x = root->getUpRight().x;
    if(root->getUpRight().y > max_y)
        max_y = root->getUpRight().y;

    for(auto it=block_map.begin(); it!=block_map.end(); it++){
        if(it == block_map.begin())//root is already placed
            continue;
        Block *block_to_place = &(it->second);
        block_to_place->left_child = nullptr;//let every block has nullptr before placing it
        block_to_place->right_child = nullptr;
        if(cur->getUpRight().x + block_to_place->w <= outline_w){//right side of b won't be outside outline
            //update ptr
            cur->left_child = block_to_place;
            block_to_place->parent = cur;
            //update coordinate
            block_to_place->origin.x = cur->origin.x + cur->w;
            block_to_place->origin.y = getYPlacement(block_to_place);
            updateContour(block_to_place);

            if(block_to_place->getUpRight().x > max_x)
                max_x = block_to_place->getUpRight().x;
            if(block_to_place->getUpRight().y > max_y)
                max_y = block_to_place->getUpRight().y;

            cur = cur->left_child;
        }
        else{
            cur = left_parent;//move cur to the left most parent of this left child tree
            cur->right_child = block_to_place;//place in the right child(top of cur)
            block_to_place->parent = cur;
            block_to_place->origin.x = cur->origin.x;
            block_to_place->origin.y = getYPlacement(block_to_place);
            updateContour(block_to_place);

            if(block_to_place->getUpRight().x > max_x)
                max_x = block_to_place->getUpRight().x;
            if(block_to_place->getUpRight().y > max_y)
                max_y = block_to_place->getUpRight().y;

            cur = cur->right_child;
            left_parent = left_parent->right_child;//lift the left parent up a block
        }
        block_name_vec.push_back(block_to_place->name);
    }
    reFloorPlan(); 
}

void BStarTree::updateContour(Block *block_placed){
    bool inserted = false;
    for(int i=0; i<up_contour.size(); i++){
        Block *b = up_contour[i];
        if(b->getUpRight().x > block_placed->origin.x && b->origin.x < block_placed->getUpRight().x){
            if(b->getUpRight().x <= block_placed->getUpRight().x){
                //printf("erase %s\n", up_contour[i]->name.c_str());
                up_contour.erase(up_contour.begin()+i);
            }
            if(!inserted){
                up_contour.insert(up_contour.begin()+i, block_placed);//up_contour contains block pointers
                //printf("insert %s\n", block_placed->name.c_str());
                inserted = true;
            }
        }
    }
    if(!inserted){
        up_contour.push_back(block_placed);
        //printf("pushback %s\n", block_placed->name.c_str());
        inserted = true;
    }
}

void BStarTree::printUpContour(){
    for(auto &b_ptr: up_contour){
        b_ptr->print();
    }
}

int BStarTree::getYPlacement(Block *block_to_place){
    int res = 0;
    bool above_single = true, intersected = false;

    for(int i=0; i<up_contour.size(); i++){
        Block *b_ptr = up_contour[i];
        if((b_ptr->getUpRight().x > block_to_place->origin.x) && (b_ptr->origin.x < block_to_place->getUpRight().x)){//block to place is intersecting with b
            intersected = true;
            if(b_ptr->getUpRight().x >= block_to_place->getUpRight().x){//block to place is totally inside b or b is the last in contour
                if(above_single)
                    return b_ptr->getUpRight().y;
                else
                    return (b_ptr->getUpRight().y > res) ? b_ptr->getUpRight().y : res;
            }
            else{//right side of block to place is not in b
                above_single = false;
                if(b_ptr->getUpRight().y > res)
                    res = b_ptr->getUpRight().y;
            }
        }
    }
    if(intersected)
        return res;
    else
        return 0;//if no block intersect with block to place in contour, place on 0
}

/*
void BStarTree::reFloorPlan(){
    max_x = 0, max_y = 0;

    root->origin.x = 0, root->origin.y = 0;
    up_contour.clear();
    updateContour(root);//put root inside contour

    if(root->getUpRight().x > max_x)
        max_x = root->getUpRight().x;
    if(root->getUpRight().y > max_y)
        max_y = root->getUpRight().y;

    Block *cur_left, *cur_right = root;
    while(cur_right != nullptr){
        cur_left = cur_right;
        if(cur_left != root){
            cur_left->origin.x = cur_left->parent->origin.x;
            cur_left->origin.y = getYPlacement(cur_left);

            if(cur_left->getUpRight().x > max_x)
                max_x = cur_left->getUpRight().x;
            if(cur_left->getUpRight().y > max_y)
                max_y = cur_left->getUpRight().y;

            updateContour(cur_left);
        }
        while(cur_left->left_child != nullptr){
            cur_left->left_child->origin.x = cur_left->getUpRight().x;
            cur_left->left_child->origin.y = getYPlacement(cur_left->left_child);
            updateContour(cur_left->left_child);

            if(cur_left->left_child->getUpRight().x > max_x)
                max_x = cur_left->left_child->getUpRight().x;
            if(cur_left->left_child->getUpRight().y > max_y)
                max_y = cur_left->left_child->getUpRight().y;

            cur_left = cur_left->left_child;
        }
        cur_right = cur_right->right_child;
    }
    printf("finish refloorplanning\n");
}
*/

void BStarTree::reFloorPlan(){
    root->origin.x = 0, root->origin.y = 0;
    up_contour.clear();
    updateContour(root);
    max_x = 0, max_y = 0;

    if(root->getUpRight().x > max_x)
        max_x = root->getUpRight().x;
    if(root->getUpRight().y > max_y)
        max_y = root->getUpRight().y;

    if(root->left_child != nullptr)
        reFloorPlanTraverse(root->left_child);
    if(root->right_child != nullptr)
        reFloorPlanTraverse(root->right_child);
}

void BStarTree::reFloorPlanTraverse(Block *block_to_place){
    if(block_to_place == block_to_place->parent->left_child){
        block_to_place->origin.x = block_to_place->parent->getUpRight().x;
        block_to_place->origin.y = getYPlacement(block_to_place);
    }
    else{
        block_to_place->origin.x = block_to_place->parent->origin.x;
        block_to_place->origin.y = getYPlacement(block_to_place);
    }
    updateContour(block_to_place);

    if(block_to_place->getUpRight().x > max_x)
        max_x = block_to_place->getUpRight().x;
    if(block_to_place->getUpRight().y > max_y)
        max_y = block_to_place->getUpRight().y;

    if(block_to_place->left_child != nullptr)
        reFloorPlanTraverse(block_to_place->left_child);
    if(block_to_place->right_child != nullptr)
        reFloorPlanTraverse(block_to_place->right_child);
}

void BStarTree::rotate(Block *aim_block){
    int temp;
    temp = aim_block->w;
    aim_block->w = aim_block->h;
    aim_block->h = temp;
}

void BStarTree::swapBlock(Block *aim_block1, Block *aim_block2){
    Block tmp;
    //update outsiders
    if(aim_block1 != root){
        if(aim_block1 == aim_block1->parent->right_child)
            aim_block1->parent->right_child = aim_block2;
        else
            aim_block1->parent->left_child = aim_block2;
    }

    if(aim_block2 != root){
        if(aim_block2 == aim_block2->parent->right_child)
            aim_block2->parent->right_child = aim_block1;
        else
            aim_block2->parent->left_child = aim_block1;
    }

    //update the 2 blocks
    tmp.right_child = aim_block1->right_child;
    aim_block1->right_child = aim_block2->right_child;
    aim_block2->right_child = tmp.right_child;

    tmp.left_child = aim_block1->left_child;
    aim_block1->left_child = aim_block2->left_child;
    aim_block2->left_child = tmp.left_child;

    tmp.parent = aim_block1->parent;
    aim_block1->parent = aim_block2->parent;
    aim_block2->parent = tmp.parent;

    if(aim_block1->right_child != nullptr)
        aim_block1->right_child->parent = aim_block1;
    if(aim_block1->left_child != nullptr)
        aim_block1->left_child->parent = aim_block1;

    if(aim_block2->right_child != nullptr)
        aim_block2->right_child->parent = aim_block2;
    if(aim_block2->left_child != nullptr)
        aim_block2->left_child->parent = aim_block2;

    if(aim_block1 == root)
        root = aim_block2;
    else if(aim_block2 == root)
        root = aim_block1;
}

void BStarTree::moveBlock(Block *aim_block, std::unordered_map<std::string, Block> &block_map){
    deleteBlock(aim_block);
    aim_block->parent = nullptr;
    aim_block->left_child = nullptr;
    aim_block->right_child = nullptr;

    int mv_index;
    bool r_or_l;
    Block *mv_parent, *child;
    std::string rand_name;

    mv_index = rand() % block_map.size();
    rand_name = block_name_vec[mv_index];
    while(rand_name == aim_block->name){//random until the parent of move block is not itself
        mv_index = rand() % block_map.size();
        rand_name = block_name_vec[mv_index];
    }

    mv_parent = &(block_map.at(rand_name));//randomly choose a parent
    r_or_l = rand() % 2;//1 for left, 0 for right
    if(!r_or_l){//right
        if(mv_parent->right_child != nullptr){
            aim_block->right_child = mv_parent->right_child;
            mv_parent->right_child->parent = aim_block;
        }
        mv_parent->right_child = aim_block;
        aim_block->parent = mv_parent;
    }
    else{//left
        if(mv_parent->left_child!= nullptr){
            aim_block->left_child = mv_parent->left_child;
            mv_parent->left_child->parent = aim_block;
        }
        mv_parent->left_child = aim_block;
        aim_block->parent = mv_parent;
    }
}

void BStarTree::deleteBlock(Block *aim_block){
    //the above two are base cases
    if(aim_block != root){
        if(aim_block->right_child == nullptr && aim_block->left_child == nullptr){
            if(aim_block == aim_block->parent->right_child)
                aim_block->parent->right_child = nullptr;
            else
                aim_block->parent->left_child = nullptr;
        }
        else if(aim_block->right_child == nullptr || aim_block->left_child == nullptr){//1 child
            if(aim_block == aim_block->parent->right_child){
                if(aim_block->right_child != nullptr){
                    aim_block->parent->right_child = aim_block->right_child;
                    aim_block->right_child->parent = aim_block->parent;
                }
                else{
                    aim_block->parent->right_child = aim_block->left_child;
                    aim_block->left_child->parent = aim_block->parent;
                }
            }
            else{
                if(aim_block->right_child != nullptr){
                    aim_block->parent->left_child = aim_block->right_child;
                    aim_block->right_child->parent = aim_block->parent;
                }
                else{
                    aim_block->parent->left_child = aim_block->left_child;
                    aim_block->left_child->parent = aim_block->parent;
                }
            }
        }
        else{//2 child
            bool r_or_l;
            Block *child;
            r_or_l = rand() % 2;//1 for left, 0 for right
            if(r_or_l)//left
                child = aim_block->left_child;
            else
                child = aim_block->right_child;
            swapBlock(aim_block, child);
            deleteBlock(aim_block);
        }
    }
    else{
        if(aim_block->right_child == nullptr && aim_block->left_child == nullptr){
            return;
        }
        else if(aim_block->right_child == nullptr || aim_block->left_child == nullptr){//1 child
            if(aim_block->right_child != nullptr){
                aim_block->right_child->parent = nullptr;
                root = aim_block->right_child;
            }
            else{
                aim_block->left_child->parent = nullptr;
                root = aim_block->left_child;
            }
        }
        else{//2 child
            bool r_or_l;
            Block *child;
            r_or_l = rand() % 2;//1 for left, 0 for right
            if(r_or_l)//left
                child = aim_block->left_child;
            else
                child = aim_block->right_child;
            swapBlock(aim_block, child);
            deleteBlock(aim_block);
        }

    }
}

int BStarTree::getArea(){
    area = max_x * max_y;
    return area;
}

int BStarTree::getWireLength(std::vector<Net> &net_vec, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map){
    wire_length = 0;

    for(auto &n: net_vec){
        double left_x = -1, right_x = -1, up_y = -1, down_y = -1;//reset to 0 incase of no nets
        for(auto &pin: n.pin_list){
            double terminal_x, terminal_y;
            //printf("%s\n", pin.c_str());
            auto it_block = block_map.find(pin);
            if(it_block != block_map.end()){//found element in block map
                //it_block->second.print();
                terminal_x = it_block->second.origin.x + (it_block->second.w)/2.; 
                terminal_y = it_block->second.origin.y + (it_block->second.h)/2.; 
            }
            else{
                auto it_term = terminal_map.find(pin);
                terminal_x = it_term->second.x;
                terminal_y = it_term->second.y;
            }

            //update hpwl points
            if(terminal_x > right_x || right_x == -1)
                right_x = terminal_x;
            if(terminal_x < left_x || left_x == -1)
                left_x = terminal_x;
            if(terminal_y > up_y || up_y == -1)
                up_y = terminal_y;
            if(terminal_y < down_y || down_y == -1)
                down_y = terminal_y;

            //printf("x: %lf\n", terminal_x);
            //printf("y: %lf\n", terminal_y);

        }
        //printf("(%.1lf %.1lf) (%.1lf %.1lf)\n", right_x, left_x, up_y, down_y);
        wire_length += ((int)right_x - (int)left_x) + ((int)up_y - (int)down_y);
    }
    return wire_length;
}

int BStarTree::getOutputCost(double &alpha, std::vector<Net> &net_vec, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map){
    getArea();
    getWireLength(net_vec, block_map, terminal_map);
    return (alpha * area + (1-alpha) * wire_length);
}

int BStarTree::getActualCost(double &alpha, int outline_w, int outline_h, std::vector<Net> &net_vec, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map){
    int ratio_diff = 100* ((double)max_x/max_y - (double)outline_w/outline_h);
    int w_violate, h_violate;
    
    if(max_x > outline_w)
        w_violate = max_x - outline_w;
    else
        w_violate = 1;

    if(max_y > outline_h)
        h_violate = max_y - outline_h;
    else
        h_violate = 1;

    getArea();
    getWireLength(net_vec, block_map, terminal_map);

    //printf("area: %d\n", area);
    //printf("hpwl: %d\n", wire_length);
    //printf("violate: (w*h)%d\n", 20*w_violate*h_violate);
    //printf("ratio: %d\n", 50000*ratio_diff);

    //return (alpha * area + (1-alpha) * wire_length);
    //return (h_violate * w_violate);
    //return (alpha * area + (1-alpha) * wire_length) + 50*(h_violate * w_violate);
    return (alpha * area + (1-alpha) * wire_length) + abs(40000*(ratio_diff));
}

//=====================================================================
//                         Printing
//=====================================================================


void BStarTree::printTree(Block *block){
    printf("[");
    printf("%s ", block->name.c_str());
    //printf("%d ", block->w);
    if(block->right_child != nullptr)
        printTree(block->right_child);
    if(block->left_child != nullptr)
        printTree(block->left_child);
    printf("]");
}


void BStarTree::printTreeAPI(){
    printf("tree:\n");
    printTree(root);
    printf("\n");
}