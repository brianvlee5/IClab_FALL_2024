#include "function.h"
#include <chrono>
#include <mutex>
#include <type_traits>

Tile* findPoint(Point aim, map<int, Tile*> &tile_map){
    //printf("findpoint\n");
    //aim.print();
    Tile *tile_now;
    bool nulled = false;
    tile_now = tile_map.begin()->second;
    while(!inTile(aim, tile_now)){
        while(!inTileHorizontal(aim, tile_now)){
            //printf("H:\n");
            //printf("%d\n", tile_now->index);
            //tile_now->print();
            if((aim.x < tile_now->origin.x) && tile_now->bl != nullptr)
                tile_now = tile_now->bl;
            else if((aim.x >= tile_now->getUpRight().x) && tile_now->tr != nullptr){
                tile_now = tile_now->tr;
            }
        }
        while(!inTileVertical(aim, tile_now)){
            //printf("V:\n");
            //printf("%d\n", tile_now->index);
            //tile_now->print();
            if((aim.y < tile_now->origin.y) && tile_now->lb != nullptr)
                tile_now = tile_now->lb;
            else if((aim.y >= tile_now->getUpRight().y) && tile_now->rt != nullptr){
                tile_now = tile_now->rt;
            }
        }
    }
    return tile_now;
}

bool inTile(Point aim, Tile *tile_now){
    int x = aim.x, y = aim.y;
    return ((x >= tile_now->origin.x) && (y >= tile_now->origin.y) && (x < tile_now->getUpRight().x) && (y < tile_now->getUpRight().y));
}

bool inTileHorizontal(Point aim, Tile *tile_now){
    int x = aim.x;
    return ((x >= tile_now->origin.x) && (x < tile_now->getUpRight().x));
}

bool inTileVertical(Point aim, Tile *tile_now){
    int y = aim.y;
    return ((y >= tile_now->origin.y) && (y < tile_now->getUpRight().y));
}

void splitHorizontal(Point p, map<int, Tile*> &tile_map, int &space_idx, int outline_h){
    Tile *new_tile = new Tile(), *check;
    Tile *split_tile = findPoint((p.y == outline_h) ? Point(p.x, p.y-1) : p, tile_map);
    new_tile->origin = Point(split_tile->origin.x, p.y);//p is the top left of new tile created

    

    if(p.y == split_tile->origin.y){
        //printf("bottom blocked\n");
        return ;
    }
    if(p.y == split_tile->getUpRight().y){
        //printf("top blocked\n");
        //p.print();
        //split_tile->print();
        return ;
    }

    int h1 = (p.y - split_tile->origin.y), h2 = split_tile->h - h1;//first update the h of new_tile cause adjacent will use it
    new_tile->h = h2;
    new_tile->w = split_tile->w;
    split_tile->h = h1;

    new_tile->rt = split_tile->rt, new_tile->tr = split_tile->tr;
    new_tile->lb = split_tile, new_tile->bl = split_tile->bl;//default new bl

    //checking adjacent tiles
    //right
    if(new_tile->tr != nullptr){
        check = new_tile->tr;
        while(check->origin.y >= split_tile->origin.y){
            if((check->origin.y >= new_tile->origin.y) && (check->origin.y < new_tile->getUpRight().y))//origin y of check is in range of new tile
                check->bl = new_tile;
            else
                check->bl = split_tile;

            if((split_tile->getUpRight().y > check->origin.y) && (split_tile->getUpRight().y <= check->getUpRight().y))
                split_tile->tr = check;

            if(check->lb != nullptr)
                check = check->lb;
            else
                break;
        }
        if((split_tile->getUpRight().y > check->origin.y) && (split_tile->getUpRight().y <= check->getUpRight().y))
            split_tile->tr = check;
    }

    //top
    if(new_tile->rt != nullptr){
        check = new_tile->rt;
        while(check->origin.x >= new_tile->origin.x){
            check->lb = new_tile;
            if(check->bl != nullptr)
                check = check->bl;
            else
                break;
        }
    }

    //left
    if(new_tile->bl != nullptr){
        check = new_tile->bl;
        while(check->getUpRight().y <= new_tile->getUpRight().y){
            if((check->getUpRight().y > new_tile->origin.y) && (check->getUpRight().y <= new_tile->getUpRight().y))//upright y of check is in range of new tile
                check->tr = new_tile;
            else
                check->tr = split_tile;

            if((new_tile->origin.y >= check->origin.y) && (new_tile->origin.y < check->getUpRight().y))
                new_tile->bl = check;

            if(check->rt != nullptr)
                check = check->rt;
            else
                break;
        }
        if((new_tile->origin.y >= check->origin.y) && (new_tile->origin.y < check->getUpRight().y))//for the case that doesn't go in while
            new_tile->bl = check;
    }
    split_tile->rt = new_tile;

    //parameters

    new_tile->index = --space_idx;
    tile_map.insert(pair<int, Tile*>(new_tile->index, new_tile));

    //split_tile->print();
    //new_tile->print();
}

void splitVertical(Tile *split_tile, Point p_left, Point p_right, map<int, Tile*> &tile_map, int &space_idx){
    Tile *new_1, *new_2;
    new_1 = new Tile(), new_2 = new Tile();
    new_1->tr = new_2, new_1->bl = split_tile, new_1->lb = split_tile->lb;//default new lb
    new_2->rt = split_tile->rt, new_2->tr = split_tile->tr, new_2->bl = new_1, new_2->lb = split_tile->lb;//default new lb
    split_tile->tr = new_1;

    //first update the h&w of new tiles cause will be used in adjacent
    new_1->h = split_tile->h, new_2->h = split_tile->h;
    new_1->w = p_right.x - p_left.x;
    new_2->w = split_tile->getUpRight().x - p_right.x;
    split_tile->w = p_left.x - split_tile->origin.x;

    new_1->origin = Point(split_tile->origin.x + split_tile->w, split_tile->origin.y);
    new_2->origin = Point(new_1->origin.x + new_1->w, new_1->origin.y);

    Tile *check;
    //bottom; check rt
    if(split_tile->lb != nullptr){
        check = split_tile->lb;
        while(check->getUpRight().x <= new_2->getUpRight().x){
            if(check->getUpRight().x > split_tile->origin.x && check->getUpRight().x <= split_tile->getUpRight().x)
                check->rt = split_tile;
            else if(check->getUpRight().x > new_1->origin.x && check->getUpRight().x <= new_1->getUpRight().x)
                check->rt = new_1;
            else
                check->rt = new_2;

            if(new_1->origin.x >= check->origin.x && new_1->origin.x < check->getUpRight().x)
                new_1->lb = check;
            if(new_2->origin.x >= check->origin.x && new_2->origin.x < check->getUpRight().x)
                new_2->lb = check;

            if(check->tr != nullptr)
                check = check->tr;
            else
                break;
        }
        if(new_1->origin.x >= check->origin.x && new_1->origin.x < check->getUpRight().x)
            new_1->lb = check;
        if(new_2->origin.x >= check->origin.x && new_2->origin.x < check->getUpRight().x)
            new_2->lb = check;
    }

    //right
    if(new_2->tr != nullptr){
        check = new_2->tr;
        while(check->origin.y >= new_2->origin.y){
            check->bl = new_2;

            if(check->lb != nullptr)
                check = check->lb;
            else
                break;
        }
    }

    //top
    if(new_2->rt != nullptr){
        check = new_2->rt;
        while(check->origin.x >= split_tile->origin.x){
            if(check->origin.x >= split_tile->origin.x && check->origin.x < split_tile->getUpRight().x)
                check->lb = split_tile;
            else if(check->origin.x >= new_1->origin.x && check->origin.x < new_1->getUpRight().x)
                check->lb = new_1;
            else
                check->lb = new_2;

            if(new_1->getUpRight().x > check->origin.x && new_1->getUpRight().x <= check->getUpRight().x)
                new_1->rt = check;
            if(split_tile->getUpRight().x > check->origin.x && split_tile->getUpRight().x <= check->getUpRight().x)
                split_tile->rt = check;

            if(check->bl != nullptr)
                check = check->bl;
            else
                break;
        }
        if(new_1->getUpRight().x > check->origin.x && new_1->getUpRight().x <= check->getUpRight().x)
            new_1->rt = check;
        if(split_tile->getUpRight().x > check->origin.x && split_tile->getUpRight().x <= check->getUpRight().x)
            split_tile->rt = check;
    }

    new_1->index = --space_idx;
    new_2->index = --space_idx;

    tile_map.insert(pair<int, Tile*>(new_1->index, new_1));
    tile_map.insert(pair<int, Tile*>(new_2->index, new_2));

    if(split_tile->rt != nullptr){
        if(split_tile->w == split_tile->rt->w && split_tile->origin.x == split_tile->rt->origin.x && !split_tile->rt->solid)//same width and the block above is a space
            merge(split_tile, split_tile->rt, tile_map);
    }
    if(new_1->rt != nullptr){
        if(new_1->w == new_1->rt->w && new_1->origin.x == new_1->rt->origin.x && !new_1->rt->solid)//same width and the block above is a space
            merge(new_1, new_1->rt, tile_map);
    }
    if(new_2->rt != nullptr){
        if(new_2->w == new_2->rt->w && new_2->origin.x == new_2->rt->origin.x && !new_2->rt->solid)//same width and the block above is a space
            merge(new_2, new_2->rt, tile_map);
    }
    if(split_tile->lb != nullptr){
        if(split_tile->w == split_tile->lb->w && split_tile->origin.x == split_tile->lb->origin.x && !split_tile->lb->solid)//same width and the block above is a space
            merge(split_tile->lb, split_tile, tile_map);
    }
    if(new_1->lb != nullptr){
        if(new_1->w == new_1->lb->w && new_1->origin.x == new_1->lb->origin.x && !new_1->lb->solid)//same width and the block above is a space
            merge(new_1->lb, new_1, tile_map);
    }
    if(new_2->lb != nullptr){
        if(new_2->w == new_2->lb->w && new_2->origin.x == new_2->lb->origin.x && !new_2->lb->solid)//same width and the block above is a space
            merge(new_2->lb, new_2, tile_map);
    }

}

void splitVerticalNoRight(Tile *split_tile, Point p_left, map<int, Tile*> &tile_map, int &space_idx){
    Tile *new_1;
    new_1 = new Tile();
    new_1->tr = split_tile->tr, new_1->rt = split_tile->rt, new_1->bl = split_tile, new_1->lb = split_tile->lb;//default new lb
    split_tile->tr = new_1;

    //first update the h&w of new tiles cause will be used in adjacent
    new_1->h = split_tile->h;
    new_1->w = split_tile->getUpRight().x - p_left.x;
    split_tile->w = p_left.x - split_tile->origin.x;

    new_1->origin = Point(split_tile->origin.x + split_tile->w, split_tile->origin.y);

    Tile *check;
    //bottom; check rt
    if(split_tile->lb != nullptr){
        check = split_tile->lb;
        while(check->getUpRight().x <= new_1->getUpRight().x){
            if(check->getUpRight().x > split_tile->origin.x && check->getUpRight().x <= split_tile->getUpRight().x)
                check->rt = split_tile;
            else
                check->rt = new_1;

            if(new_1->origin.x >= check->origin.x && new_1->origin.x < check->getUpRight().x)
                new_1->lb = check;

            if(check->tr != nullptr)
                check = check->tr;
            else
                break;
        }
        if(new_1->origin.x >= check->origin.x && new_1->origin.x < check->getUpRight().x)
            new_1->lb = check;
    }

    //right
    if(new_1->tr != nullptr){
        check = new_1->tr;
        while(check->origin.y >= new_1->origin.y){
            check->bl = new_1;

            if(check->lb != nullptr)
                check = check->lb;
            else
                break;
        }
    }

    //top
    if(new_1->rt != nullptr){
        check = new_1->rt;
        while(check->origin.x >= split_tile->origin.x){
            if(check->origin.x >= split_tile->origin.x && check->origin.x < split_tile->getUpRight().x)
                check->lb = split_tile;
            else
                check->lb = new_1;

            if(split_tile->getUpRight().x > check->origin.x && split_tile->getUpRight().x <= check->getUpRight().x)
                split_tile->rt = check;

            if(check->bl != nullptr)
                check = check->bl;
            else
                break;
        }
        if(split_tile->getUpRight().x > check->origin.x && split_tile->getUpRight().x <= check->getUpRight().x)
            split_tile->rt = check;
    }

    new_1->index = --space_idx;

    tile_map.insert(pair<int, Tile*>(new_1->index, new_1));

    if(split_tile->rt != nullptr){
        if(split_tile->w == split_tile->rt->w && split_tile->origin.x == split_tile->rt->origin.x && !split_tile->rt->solid)//same width and the block above is a space
            merge(split_tile, split_tile->rt, tile_map);
    }
    if(new_1->rt != nullptr){
        if(new_1->w == new_1->rt->w && new_1->origin.x == new_1->rt->origin.x && !new_1->rt->solid)//same width and the block above is a space
            merge(new_1, new_1->rt, tile_map);
    }
    if(split_tile->lb != nullptr){
        if(split_tile->w == split_tile->lb->w && split_tile->origin.x == split_tile->lb->origin.x && !split_tile->lb->solid)//same width and the block above is a space
            merge(split_tile->lb, split_tile, tile_map);
    }
    if(new_1->lb != nullptr){
        if(new_1->w == new_1->lb->w && new_1->origin.x == new_1->lb->origin.x && !new_1->lb->solid)//same width and the block above is a space
            merge(new_1->lb, new_1, tile_map);
    }
}

void splitVerticalNoLeft(Tile *split_tile, Point p_right, map<int, Tile*> &tile_map, int &space_idx){
    Tile *new_1;
    new_1 = new Tile();
    new_1->tr = split_tile->tr, new_1->rt = split_tile->rt, new_1->bl = split_tile, new_1->lb = split_tile->lb;//default new lb
    split_tile->tr = new_1;

    //first update the h&w of new tiles cause will be used in adjacent
    new_1->h = split_tile->h;
    new_1->w = split_tile->getUpRight().x - p_right.x;
    split_tile->w = p_right.x - split_tile->origin.x;

    new_1->origin = Point(split_tile->origin.x + split_tile->w, split_tile->origin.y);

    Tile *check;
    //bottom; check rt
    if(split_tile->lb != nullptr){
        check = split_tile->lb;
        while(check->getUpRight().x <= new_1->getUpRight().x){
            if(check->getUpRight().x > split_tile->origin.x && check->getUpRight().x <= split_tile->getUpRight().x)
                check->rt = split_tile;
            else
                check->rt = new_1;

            if(new_1->origin.x >= check->origin.x && new_1->origin.x < check->getUpRight().x)
                new_1->lb = check;

            if(check->tr != nullptr)
                check = check->tr;
            else
                break;
        }
        if(new_1->origin.x >= check->origin.x && new_1->origin.x < check->getUpRight().x)
            new_1->lb = check;
    }

    //right
    if(new_1->tr != nullptr){
        check = new_1->tr;
        while(check->origin.y >= new_1->origin.y){
            check->bl = new_1;

            if(check->lb != nullptr)
                check = check->lb;
            else
                break;
        }
    }

    //top
    if(new_1->rt != nullptr){
        check = new_1->rt;
        while(check->origin.x >= split_tile->origin.x){
            if(check->origin.x >= split_tile->origin.x && check->origin.x < split_tile->getUpRight().x)
                check->lb = split_tile;
            else
                check->lb = new_1;

            if(split_tile->getUpRight().x > check->origin.x && split_tile->getUpRight().x <= check->getUpRight().x)
                split_tile->rt = check;

            if(check->bl != nullptr)
                check = check->bl;
            else
                break;
        }
        if(split_tile->getUpRight().x > check->origin.x && split_tile->getUpRight().x <= check->getUpRight().x)
            split_tile->rt = check;
    }

    new_1->index = --space_idx;

    tile_map.insert(pair<int, Tile*>(new_1->index, new_1));

    if(split_tile->rt != nullptr){
        if(split_tile->w == split_tile->rt->w && split_tile->origin.x == split_tile->rt->origin.x && !split_tile->rt->solid)//same width and the block above is a space
            merge(split_tile, split_tile->rt, tile_map);
    }
    if(new_1->rt != nullptr){
        if(new_1->w == new_1->rt->w && new_1->origin.x == new_1->rt->origin.x && !new_1->rt->solid)//same width and the block above is a space
            merge(new_1, new_1->rt, tile_map);
    }
    if(split_tile->lb != nullptr){
        if(split_tile->w == split_tile->lb->w && split_tile->origin.x == split_tile->lb->origin.x && !split_tile->lb->solid)//same width and the block above is a space
            merge(split_tile->lb, split_tile, tile_map);
    }
    if(new_1->lb != nullptr){
        if(new_1->w == new_1->lb->w && new_1->origin.x == new_1->lb->origin.x && !new_1->lb->solid)//same width and the block above is a space
            merge(new_1->lb, new_1, tile_map);
    }
}

 
void merge(Tile *new_tile, Tile *old_tile, map<int, Tile*> &tile_map){
    new_tile->tr = old_tile->tr, new_tile->rt = old_tile->rt;
    new_tile->h += old_tile->h;
    Tile *check;
    if(new_tile->bl != nullptr){
        check = new_tile->bl;
        while(check->getUpRight().y <= new_tile->getUpRight().y){
            check->tr = new_tile;
            if(check->rt != nullptr)
                check = check->rt;
            else
                break;
        }
    }

    if(new_tile->rt != nullptr){
        check = new_tile->rt;
        while(check->origin.x >= new_tile->origin.x){
            check->lb = new_tile;
            if(check->bl != nullptr)
                check = check->bl;
            else
                break;
        }
    }

    if(new_tile->tr != nullptr){
        check = new_tile->tr;
        while(check->origin.y >= new_tile->origin.y){
            check->bl = new_tile;
            if(check->lb != nullptr)
                check = check->lb;
            else
                break;
        }
    }

    //remove the old tile
    tile_map.erase(old_tile->index);
    //printf("remove: %d\n", old_tile->index);
    delete old_tile;//can't use delete[]
}

Tile *createTile(Point ori, int w, int h, map<int, Tile*> &tile_map, int &space_idx, int new_index, int outline_w, int outline_h){
    if(ori.x + w > outline_w){
        cerr << "x out of bound\n";
    }
    if(ori.y + h > outline_h){
        cerr << "h out of bound\n";
    }

    Point tl(ori.x, ori.y+h), tr(ori.x+w, ori.y+h), bl(ori.x, ori.y), br(ori.x+w, ori.y);//tl top left
    Tile *up_space, *down_space;
    splitHorizontal(tl, tile_map, space_idx, outline_h);
    //printf("upsplit\n");
    splitHorizontal(bl, tile_map, space_idx, outline_h);
    //printf("downsplit\n");

    Point scan_p = tl;
    Tile *space_to_split, *new_tile, *test;
    while(scan_p.y != bl.y){//spliting the sides
        scan_p.y -= 1;
        space_to_split = findPoint(scan_p, tile_map);
        scan_p.y = space_to_split->origin.y;
        if(tl.x != space_to_split->origin.x && tr.x != space_to_split->getUpRight().x)
            splitVertical(space_to_split, bl, br, tile_map, space_idx);
        else if(tl.x != space_to_split->origin.x && tr.x == space_to_split->getUpRight().x)
            splitVerticalNoRight(space_to_split, bl, tile_map, space_idx);
        else if(tl.x == space_to_split->origin.x && tr.x != space_to_split->getUpRight().x)
            splitVerticalNoLeft(space_to_split, br, tile_map, space_idx);

        //if(new_index == 480){
        //    printf("-----test-----\n");
        //    test = findPoint(scan_p, tile_map);
        //    test->print();
        //    test->rt->print();
        //    printf("------\n");
        //}
    }

    splitHorizontal(tl, tile_map, space_idx, outline_h);
    splitHorizontal(bl, tile_map, space_idx, outline_h);

    new_tile = findPoint(scan_p, tile_map);

    tile_map.erase(new_tile->index);
    new_tile->solid = 1;
    new_tile->index = new_index;
    tile_map.insert(pair<int, Tile*>(new_tile->index, new_tile));

    return new_tile;
}

void outputLayout(ofstream &output, map<int, Tile*> &tile_map, int outline_w, int outline_h){
    output << tile_map.size() << "\n";
    output << outline_w << " " << outline_h << "\n";
    for(auto &m : tile_map){
        Tile *tmp = m.second;
        output << tmp->index << " " << tmp->origin.x << " " << tmp->origin.y << " " << tmp->w << ' ' << tmp->h << "\n";
    }
}