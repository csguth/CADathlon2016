#include <iostream>
#include <GTL/graph.h>
#include<vector>
#include<list>

using location_type = double;
using id_type = int;
using width_type = unsigned;
using coordinate_type = unsigned;

#define INVALID_ID -1

class cell{
    public:
        cell() : location(std::make_pair(0.0, 0.0)), id(INVALID_ID), width(0)
        {
        }
        
        cell(std::pair<location_type, location_type> loc, id_type id, width_type width) : location(loc), id(id), width(width)
        {
        }
        
        std::pair<location_type, location_type> location;
        id_type id;
        width_type width;
};

class row{
    public:
        row() : number_of_sites(0), starting_y_coordinate(0)
        {
        }

        row(size_t num_sites, unsigned y_coord) : number_of_sites(num_sites), starting_y_coordinate(y_coord)
        {
        }

        size_t number_of_sites;
        coordinate_type starting_y_coordinate;
        std::list<id_type> cell_ids_in_current_row;
};  

class placement{
    public: 
        placement(size_t num_cells, size_t num_rows) : cells(num_cells), rows(num_rows)
        {
        }
        
        std::vector<cell> cells;
        std::vector<row> rows;
};

int main() {
    std::cout << "hello world" << std::endl;

    graph G;
    G.new_node();

    return 0;
}
