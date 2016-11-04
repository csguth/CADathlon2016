#include <iostream>
#include "placement.h"
#include <assert.h>
#include "graph.h"

int main() {
    std::cout << "TPL Detailed Placement" << std::endl;

    Placement placement(5, 2, 5);
    //Add Rows
    id_type r1 = placement.rows.add(20, 0);
    id_type r2 = placement.rows.add(20, 0);
    id_type r3 = placement.rows.add(20, 0);
    id_type r4 = placement.rows.add(20, 0);
    id_type r5 = placement.rows.add(20, 0);
    //Add Cells
    id_type c1 = placement.cells.add(std::make_pair(0,0), 5);
    id_type c2 = placement.cells.add(std::make_pair(10,0), 8);
    id_type c3 = placement.cells.add(std::make_pair(0,30), 6);
    id_type c4 = placement.cells.add(std::make_pair(10,30), 5);
    id_type c5 = placement.cells.add(std::make_pair(5,40), 3);
    id_type c6 = placement.cells.add(std::make_pair(14,40), 5);
    //Add Nets
    id_type n1 = placement.nets.add({c1, c2});
    id_type n2 = placement.nets.add({c3, c4, c5, c6});

    assert(placement.hpwl(n1) == 10);
    assert(placement.hpwl(n2) == 24);


    std::cout << "Done! " << std::endl;
    return 0;
}
