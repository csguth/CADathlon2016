#include<vector>
#include<list>
#include<limits>
#include<array>

using location_type = std::pair<unsigned,unsigned>;
using id_type = int;
using width_type = unsigned;
using coordinate_type = unsigned;
using net_type = std::vector<id_type>;

#define INVALID_ID -1

class Cells{
    public:
        Cells() : locations(0), widths(0)
        {
        }
        
        void preallocate(size_t size)
        {
            locations.reserve(size);
            widths.reserve(size);
        }

        id_type add(location_type location, width_type width)
        {
            locations.push_back(location);
            widths.push_back(width);
            return locations.size()-1;
        }
        
        std::vector<location_type> locations; //lower left corner
        std::vector<width_type> widths;
};

class Nets{
    public:
        Nets() : net_cells(0)
        {
        }

        void preallocate(size_t size)
        {
            net_cells.reserve(size);
        }

        id_type add(net_type&& net)
        {
            net_cells.emplace_back(net);
            return net_cells.size()-1;
        }

        std::vector<net_type> net_cells;
};

class Rows{
    public:
        Rows() : number_of_sites(0), starting_y_coordinates(0)
        {
        }

        void preallocate(size_t size)
        {
            if(size > number_of_sites.size())
            {
                number_of_sites.reserve(size);
                starting_y_coordinates.reserve(size);
            }
        }

        size_t num_sites(id_type row){return number_of_sites.at(row);}

        id_type add(size_t num_sites, coordinate_type y_coordinate)
        {
            number_of_sites.push_back(num_sites);
            starting_y_coordinates.push_back(y_coordinate);
        }
        

        std::vector<size_t> number_of_sites;
        std::vector<coordinate_type> starting_y_coordinates;
        //std::list<id_type> cell_ids_in_current_row;
};  

class Placement{
    public: 
        Placement()
        {
        }
                                                                        
        Placement(size_t num_cells, size_t num_nets, size_t num_rows)
        {
            cells.preallocate(num_cells);
            nets.preallocate(num_nets);
            rows.preallocate(num_rows);
        }

        coordinate_type hpwl(id_type net_id)
        {
            double hpwl = 0;
            const net_type & net = nets.net_cells.at(net_id);

            coordinate_type min_x, min_y, max_x, max_y;
            min_x = min_y = std::numeric_limits<coordinate_type>::max();
            max_x = max_y = std::numeric_limits<coordinate_type>::min();
            for(auto id : net)
            {
                min_x = std::min(min_x, cells.locations.at(id).first);
                min_y = std::min(min_y, cells.locations.at(id).second);
                max_x = std::max(max_x, cells.locations.at(id).first);
                max_y = std::max(max_y, cells.locations.at(id).second);
            }
            return (max_x-min_x) + (max_y-min_y);
        }

        size_t num_sites(id_type row){return rows.num_sites(row);}
        
        Cells cells;
        Nets nets;
        Rows rows;
};

using cell = size_t;
using color = size_t;

//0 to n-1
double lut_min_distance(cell i, color p, cell j, color q)
{
    static constexpr size_t num_colors = 2;
    static constexpr size_t num_cells = 2;
    static constexpr size_t lut_width = num_colors*num_cells;
    static constexpr std::array<int, lut_width*lut_width> lut{
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}
    };

    size_t column_index = i*num_colors+p;
    size_t row_index = j*num_colors+q;
    size_t index = row_index*lut_width+column_index;
    return lut[index];
}



