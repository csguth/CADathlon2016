#include <iostream>
#include <array>
#include "frame.h"

int main() {
    std::cout << "OLED Display DVS" << std::endl;

    
    static constexpr std::array<double, 16> new_red_component {
              { 255, 254, 253, 252, 251, 250, 249, 248, 100, 99, 98, 97, 96, 95, 94, 93 }
    };

    static constexpr std::array<double, 16> new_green_component {
              { 207, 206, 205, 204, 203, 202, 201, 200, 199, 50, 49, 48, 47, 46, 45, 44 }
    };

    static constexpr std::array<double, 16> new_blue_component {
              { 150, 151, 152, 153, 154, 155, 156, 157, 157, 10, 9, 8, 7, 6, 5, 4}
    };

    Frame frame;

    unsigned index=0;
    for(unsigned i = 0; i < FRAME_WIDTH; ++i)
    {
        for(unsigned j = 0; j < FRAME_HEIGHT; ++j, ++index)
            frame.red(i,j, new_red_component[index]);
    }

    index=0;
    for(unsigned i = 0; i < FRAME_WIDTH; ++i)
    {
        for(unsigned j = 0; j < FRAME_HEIGHT; ++j, ++index)
            frame.green(i,j, new_green_component[index]);
    }

    index=0;
    for(unsigned i = 0; i < FRAME_WIDTH; ++i)
    {
        for(unsigned j = 0; j < FRAME_HEIGHT; ++j, ++index)
            frame.blue(i,j, new_blue_component[index]);
    }

    frame.print();

    auto ssim = compute_ssim(frame, frame);
    std::cout << "SSIM: " << ssim << std::endl;
    assert(ssim == 1);

    auto interval_ssim = compute_interval_ssim(frame, frame, {0,1}, {0,1});
    std::cout << "Interval SSIM: " << interval_ssim << std::endl;
    assert(interval_ssim == 1);

    assert(total_sacrificed_luminance(frame, 255) == 255);
    assert(total_sacrificed_luminance(frame, 256) == 0);
    assert(total_sacrificed_luminance(frame, 0) == 6374);
    assert(sacrificed_luminance_ratio(frame,0) == 1);

    std::cout << "Total sacrificed luminance: " << total_sacrificed_luminance(frame, 128) << std::endl;
    std::cout << "Total sacrificed luminance: " << total_sacrificed_luminance(frame, 0) << std::endl;

    std::cout << "Sacrificed luminance ratio: " << sacrificed_luminance_ratio(frame, 128) << std::endl;
    std::cout << "Sacrificed luminance ratio: " << sacrificed_luminance_ratio(frame, 254) << std::endl;
   
    std::cout << std::endl;
    //applying spatial vdd optimization
    Vdd_Regions vdd_regions(4,MAX_VDD);
    std::cout << "**Initial Vdd at each Region" << std::endl;
    vdd_regions.print();

    spatial_vdd_optimization(vdd_regions, frame, 0.12);

    std::cout << "**Final Vdd at each Region" << std::endl;
    vdd_regions.print();


    std::cout << "Done!" << std::endl;
    return 0;
}
