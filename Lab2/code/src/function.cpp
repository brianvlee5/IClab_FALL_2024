#include "function.h"


void readInputBlock(std::ifstream &block_input, std::unordered_map<std::string, Block> &block_map, std::unordered_map<std::string, Point> &terminal_map, int &outline_w, int &outline_h){
    std::string dump;
    int block_num, terminal_num;

    block_input >> dump;//Outline:
    block_input >> outline_w >> outline_h;

    block_input >> dump;//NumBlocks:
    block_input >> block_num;

    block_input >> dump;//NumTerminals:
    block_input >> terminal_num;

    for(int i=0; i<block_num; i++){
        Block temp_block;
        block_input >> temp_block.name >> temp_block.w >> temp_block.h;
        block_map.insert(std::pair<std::string, Block>(temp_block.name, temp_block));
    }

    printf("finish reading blocks\n");

    for(int i=0; i<terminal_num; i++){
        Point temp_term;
        std::string term_name, terminal_dump;
        block_input >> term_name >> terminal_dump >> temp_term.x >> temp_term.y;
        terminal_map.insert(std::pair<std::string, Point>(term_name, temp_term));
    }

    printf("finish reading terminals\n");


}

void readInputNet(std::ifstream &net_input, std::vector<Net> &net_vec){
    std::string dump;
    int net_nums;
    net_input >> dump;//NumNets:
    net_input >> net_nums;

    for(int i=0; i<net_nums; i++){
        int net_degree;
        std::string pin_name;
        Net tmp_net;

        net_input >> dump;//NetDegree:
        net_input >> net_degree;
        for(int j=0; j<net_degree; j++){
            net_input >> pin_name;
            tmp_net.pin_list.push_back(pin_name);
        }
        net_vec.push_back(tmp_net);
    }
    printf("finish reading net\n");
}

void outputLayout(std::ofstream &layout_output, std::unordered_map<std::string, Block> &block_map, int outline_w, int outline_h){
    layout_output << block_map.size() << "\n";
    layout_output << outline_w << " " << outline_h << "\n";
    int cntr = 1;
    for(auto &b: block_map){
        //b.second.printAdjacent();
        layout_output << cntr << " " << b.second.origin.x << " " << b.second.origin.y << " " << b.second.w << " " << b.second.h << " " << b.first << "\n";
        cntr++;
    }
    printf("finish output layout\n");
}

void copyTree(BStarTree &original_tree, BStarTree &dest_tree, std::unordered_map<std::string, Block> &original_map, std::unordered_map<std::string, Block> &dest_map){
    dest_tree = original_tree;
    dest_map = original_map;
    dest_tree.min_area = original_tree.min_area;
    dest_tree.block_name_vec = original_tree.block_name_vec;

    dest_tree.root = &dest_map[original_tree.root->name];
    copyTreeTraverse(dest_tree.root->name, original_map, dest_map);
    dest_tree.reFloorPlan();
}

void copyTreeTraverse(std::string block_name, std::unordered_map<std::string, Block> &original_map, std::unordered_map<std::string, Block> &dest_map){
    Block *ori_ptr, *dst_ptr;
    ori_ptr = &original_map[block_name];
    dst_ptr = &dest_map[block_name];

    dst_ptr->parent = nullptr;
    dst_ptr->left_child = nullptr;
    dst_ptr->right_child = nullptr;

    dst_ptr->w = ori_ptr->w;
    dst_ptr->h = ori_ptr->h;

    if(ori_ptr->parent != nullptr)
        dst_ptr->parent = &dest_map[ori_ptr->parent->name];

    if(ori_ptr->right_child != nullptr){
        dst_ptr->right_child = &dest_map[ori_ptr->right_child->name];
        copyTreeTraverse(dst_ptr->right_child->name, original_map, dest_map);
    }
    if(ori_ptr->left_child != nullptr){
        dst_ptr->left_child = &dest_map[ori_ptr->left_child->name];
        copyTreeTraverse(dst_ptr->left_child->name, original_map, dest_map);
    }
}

void randomPerturb(BStarTree &bst, std::unordered_map<std::string, Block> &block_map){
    int opt = rand()%3;
    int rand_index1, rand_index2;
    std::string rand_name1, rand_name2;
    Block *block_to_per1, *block_to_per2;//block to perturbate

    rand_index1 = rand() % block_map.size();
    rand_name1 = bst.block_name_vec[rand_index1];
    block_to_per1 = &(block_map.at(rand_name1));

    if(opt == 0){//rotate
        bst.rotate(block_to_per1);
        bst.reFloorPlan();
        //printf("rotated complete\n");
    }
    else if(opt == 1){//move
        bst.moveBlock(block_to_per1, block_map);
        bst.reFloorPlan();
        //printf("move complete\n");
    }
    else if(opt == 2){//swap
        rand_index2 = rand() % block_map.size();
        while(rand_index2 == rand_index1){
            rand_index2 = rand() % block_map.size();
        }

        rand_name2 = bst.block_name_vec[rand_index2];
        block_to_per2 = &(block_map.at(rand_name2));
        bst.swapBlock(block_to_per1, block_to_per2);
        bst.reFloorPlan();
        //printf("swap complete\n");
    }
}

void outputReport(std::ofstream &output, BStarTree &bst, std::unordered_map<std::string, Block> &block_map, double run_time, int &cost){
    output << cost << "\n";
    output << bst.wire_length << "\n";
    output << bst.area << "\n";
    output << bst.max_x << " " << bst.max_y << '\n';
    output << run_time << '\n';
    for(auto &b : block_map){
        output << b.second.name << " " << b.second.origin.x << " " << b.second.origin.y << " " << b.second.getUpRight().x << " " << b.second.getUpRight().y << "\n";
    }
}

void updateAlpha(double &alpha, double &alpha_base, int &feasable_round, int &n_round){
    if(alpha_base > 0.2)
        alpha = alpha_base + (1-alpha_base-0.1)*(double)(n_round-feasable_round)/n_round;
    else
        alpha = alpha_base + (1-alpha_base)*(double)(n_round-feasable_round)/n_round;
    //alpha = alpha_base + (1-alpha_base)*(double)(n_round-feasable_round)/n_round;
}