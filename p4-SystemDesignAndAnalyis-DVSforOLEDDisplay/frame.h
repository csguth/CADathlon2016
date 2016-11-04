#include <vector>
#include <assert.h>
#include <numeric>
#include <cmath>
#include <array>
#include <iostream>

static constexpr unsigned FRAME_WIDTH=4;
static constexpr unsigned FRAME_HEIGHT=4;
static constexpr unsigned FRAME_SIZE=FRAME_WIDTH*FRAME_HEIGHT;
static constexpr unsigned MAXIMUM_GRAY_LEVEL=255;
static constexpr double HIGH_QUALITY_THRESHOLD=0.98;
static constexpr double MED_QUALITY_THRESHOLD=0.96;
static constexpr double LOW_QUALITY_THRESHOLD=0.94;
static constexpr double MAX_VDD=20.0;

using pixel_type = unsigned;
using frame_storage_type = std::vector<pixel_type>;

#define MAP(x,y) x*FRAME_WIDTH+y
#define MAP_WIDTH(x,y,width) x*width+y

class Frame{
    private:
        frame_storage_type red_component, green_component, blue_component;

    public:        
        Frame() : red_component(FRAME_SIZE), green_component(FRAME_SIZE), blue_component(FRAME_SIZE)
        {
        }
        
        const frame_storage_type & get_red() const {return red_component;}
        const frame_storage_type & get_green() const {return green_component;}
        const frame_storage_type & get_blue() const {return blue_component;}

        pixel_type red(unsigned x, unsigned y) const {return red_component.at(MAP(x,y));}
        pixel_type green(unsigned x, unsigned y) const {return green_component.at(MAP(x,y));}
        pixel_type blue(unsigned x, unsigned y) const {return blue_component.at(MAP(x,y));}

        void red(unsigned x, unsigned y, pixel_type value){red_component.at(MAP(x,y)) = value;}
        void green(unsigned x, unsigned y, pixel_type value){green_component.at(MAP(x,y)) = value;}
        void blue(unsigned x, unsigned y, pixel_type value){blue_component.at(MAP(x,y)) = value;}

        void print()
        {
            std::cout << "**Red Component**" << std::endl;
            for(unsigned i = 0; i < FRAME_WIDTH; ++i)
            {
                for(unsigned j = 0; j < FRAME_HEIGHT; ++j)
                    std::cout << red(i,j) << "\t";
                std::cout << std::endl;
            }
            std::cout << std::endl;

            std::cout << "**Green Component**" << std::endl;
            for(unsigned i = 0; i < FRAME_WIDTH; ++i)
            {
                for(unsigned j = 0; j < FRAME_HEIGHT; ++j)
                    std::cout << green(i,j) << "\t";
                std::cout << std::endl;
            }
            std::cout << std::endl;

            std::cout << "**Blue Component**" << std::endl;
            for(unsigned i = 0; i < FRAME_WIDTH; ++i)
            {
                for(unsigned j = 0; j < FRAME_HEIGHT; ++j)
                    std::cout << blue(i,j) << "\t";
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
};


double compute_mean(const Frame & frame)
{
    double mean = std::accumulate(frame.get_red().begin(), frame.get_red().end(), 0.0);
    mean = std::accumulate(frame.get_green().begin(), frame.get_green().end(), mean);
    mean = std::accumulate(frame.get_blue().begin(), frame.get_blue().end(), mean);
    mean = mean/(frame.get_red().size()*3);
    return mean;
}

double compute_interval_mean(const Frame & frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    double mean = 0.0;
    int den = 0;
    for(unsigned i = x_interval.first; i < x_interval.second; ++i)
    {
        for(unsigned j = y_interval.first; j < y_interval.second; ++j, ++den)
            mean += frame.red(i,j) + frame.green(i,j) + frame.blue(i,j);
    }
    mean = mean/(den*3);
    return mean;
}

double compute_variance(const Frame & frame)
{
    double mean = compute_mean(frame);
    double variance = 0.0;
    for(auto value : frame.get_red())
        variance += ((value - mean)*(value - mean));
    for(auto value : frame.get_green())
        variance += ((value - mean)*(value - mean));
    for(auto value : frame.get_blue())
        variance += ((value - mean)*(value - mean));

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    variance = variance/(frame.get_red().size()*3 - 1);
    return variance;
}

double compute_interval_variance(const Frame & frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    double mean = compute_interval_mean(frame, x_interval, y_interval);
    double variance = 0.0;
    int den = 0;

    for(unsigned i = x_interval.first; i < x_interval.second; ++i)
    {
        for(unsigned j = y_interval.first; j < y_interval.second; ++j, ++den)
            variance += (frame.red(i,j) - mean)*(frame.red(i,j) - mean)
                     +  (frame.green(i,j) - mean)*(frame.green(i,j) - mean)
                     +  (frame.blue(i,j) - mean)*(frame.blue(i,j) - mean);

    }

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    variance = variance/(den*3 - 1);
    return variance;
}

double compute_covariance(const Frame & new_frame, const Frame & original_frame)
{
    double new_frame_mean = compute_mean(new_frame); 
    double original_frame_mean = compute_mean(original_frame); 

    double covariance = 0.0;
    auto it_original = original_frame.get_red().begin();
    for(auto it_new = new_frame.get_red().begin(); it_new != new_frame.get_red().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);
    it_original = original_frame.get_green().begin();
    for(auto it_new = new_frame.get_green().begin(); it_new != new_frame.get_green().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);
    it_original = original_frame.get_blue().begin();
    for(auto it_new = new_frame.get_blue().begin(); it_new != new_frame.get_blue().end(); ++it_new,++it_original)
        covariance += (*it_new - new_frame_mean)*(*it_original - original_frame_mean);

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    covariance = covariance/(new_frame.get_red().size()*3 - 1);
    return covariance;
}

double compute_interval_covariance(const Frame & new_frame, const Frame & original_frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    double new_frame_mean = compute_interval_mean(new_frame, x_interval, y_interval); 
    double original_frame_mean = compute_interval_mean(original_frame, x_interval, y_interval); 

    double covariance = 0.0;
    int den = 0;

    for(unsigned i = x_interval.first; i < x_interval.second; ++i)
    {
        for(unsigned j = y_interval.first; j < y_interval.second; ++j, ++den)
            covariance += (new_frame.red(i,j) - new_frame_mean)*(original_frame.red(i,j) - original_frame_mean)
                     +  (new_frame.green(i,j) - new_frame_mean)*(original_frame.green(i,j) - original_frame_mean)
                     +  (new_frame.blue(i,j) - new_frame_mean)*(original_frame.blue(i,j) - original_frame_mean);

    }

    //it is assumed that it is a sample due to quantization. That is why we use N-1
    covariance = covariance/(den*3 - 1);
    return covariance;
}

double compute_ssim(const Frame & new_frame, const Frame & original_frame)
{
    const double C1 = 2.55;
    const double C2 = 7.56;
    double ssim = 0.0;

    double new_frame_mean = compute_mean(new_frame);
    double new_frame_variance = compute_variance(new_frame);

    double original_frame_mean = compute_mean(original_frame);
    double original_frame_variance = compute_variance(original_frame);

    double covariance = compute_covariance(new_frame, original_frame);

    ssim = ((2*new_frame_mean*original_frame_mean + C1)*(2*covariance + C2))/
                 ((std::pow(new_frame_mean,2) + std::pow(original_frame_mean,2) + C1)*(new_frame_variance + original_frame_variance + C2)); 

    return ssim;
}

double compute_interval_ssim(const Frame & new_frame, const Frame & original_frame, std::pair<unsigned, unsigned> x_interval, std::pair<unsigned, unsigned> y_interval)
{
    const double C1 = 2.55;
    const double C2 = 7.56;
    double ssim = 0.0;

    double new_frame_mean = compute_interval_mean(new_frame, x_interval, y_interval);
    double new_frame_variance = compute_interval_variance(new_frame, x_interval, y_interval);

    double original_frame_mean = compute_interval_mean(original_frame, x_interval, y_interval);
    double original_frame_variance = compute_interval_variance(original_frame, x_interval, y_interval);

    double covariance = compute_interval_covariance(new_frame, original_frame, x_interval, y_interval);

    ssim = ((2*new_frame_mean*original_frame_mean + C1)*(2*covariance + C2))/
                 ((std::pow(new_frame_mean,2) + std::pow(original_frame_mean,2) + C1)*(new_frame_variance + original_frame_variance + C2)); 

    return ssim;
}



pixel_type total_sacrificed_luminance(const Frame & frame, pixel_type threshold)
{
    pixel_type total_sacrificed_luminance = std::accumulate(frame.get_red().begin(), frame.get_red().end(), 0.0, [threshold] (pixel_type sum, pixel_type a) { return a >= threshold ? sum+a : sum; });
    total_sacrificed_luminance = std::accumulate(frame.get_green().begin(), frame.get_green().end(), total_sacrificed_luminance, [threshold] (pixel_type sum, pixel_type a) { return a >= threshold ? sum+a : sum; });
    total_sacrificed_luminance = std::accumulate(frame.get_blue().begin(), frame.get_blue().end(), total_sacrificed_luminance, [threshold] (pixel_type sum, pixel_type a) { return a >= threshold ? sum+a : sum; });
    return total_sacrificed_luminance;
}

double sacrificed_luminance_ratio(const Frame & frame, pixel_type threshold)
{
    return (double)total_sacrificed_luminance(frame, threshold)/(double)total_sacrificed_luminance(frame, 0);
}

double sacrificed_luminance_ratio(const Frame & frame, pixel_type threshold, pixel_type total)
{
    return (double)total_sacrificed_luminance(frame, threshold)/(double)total;
}

class Vdd_Regions
{   
    private:
        unsigned reg_width, reg_height;
        std::vector<double> vdd_regions;
    public:
        Vdd_Regions(unsigned number_of_regions, double init_vdd) : reg_width(number_of_regions/2), reg_height(number_of_regions/2), vdd_regions(number_of_regions, init_vdd)
        {
        }
        unsigned width(){return reg_width;}
        unsigned height(){return reg_height;}
        size_t size() const {return vdd_regions.size();}
        double vdd(unsigned x, unsigned y) const {return vdd_regions.at(MAP_WIDTH(x,y, reg_width));}
        void vdd(unsigned x, unsigned y, double vdd) {vdd_regions.at(MAP_WIDTH(x,y, reg_width)) = vdd;}

        void print()
        {
            std::cout << "**Vdd at each region**" << std::endl;
            for(unsigned i = 0; i < reg_width; ++i)
            {
                for(unsigned j = 0; j < reg_height; ++j)
                    std::cout << vdd(i,j) << "\t";
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
};

double linear_interpolation(const double y1, const double y0, const double x1, const double x0, const double x) {
    const double kA = (y1-y0)/(x1-x0);
    const double kB = y0;
    const double kX = x - x0;
    return kA*kX + kB;
}

//percentage of the maximum Luminance (255). For example, a 10V VDD achieves a maximum luminance of 128
//maps a given vdd to a gray level threshold
double lookup_gray_level_threshold(double vdd)
{
    assert(vdd <= MAX_VDD);
    assert(vdd >= 0);
    constexpr std::array<double, 21> GL_threshold {
        {0.0, 0.5, 0.1, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95, 1.0 }
    };

    unsigned prev_index = std::floor(vdd);
    unsigned next_index = std::ceil(vdd);
    if(prev_index == next_index)
        return GL_threshold[prev_index];
    else
        return linear_interpolation(GL_threshold[next_index], GL_threshold[prev_index], next_index, prev_index, vdd);
}

void spatial_vdd_optimization(Vdd_Regions & region, const Frame & frame, double max_sacrificed_lum_ratio)
{
    const double vdd_step = 0.1;
    const pixel_type total_luminance = total_sacrificed_luminance(frame, 0);
    
    std::cout << "Spatial VDD optimization" << std::endl; 
    for(unsigned i = 0; i < region.width(); ++i)
    {
        for(unsigned j = 0; j < region.height(); ++j)
        {
            double region_vdd = MAX_VDD;
            unsigned max_gray_level = std::floor(MAXIMUM_GRAY_LEVEL*lookup_gray_level_threshold(region_vdd));
            double sr =  sacrificed_luminance_ratio(frame, max_gray_level, total_luminance);
            
            std::cout << "Region: " << i << "," << j << std::endl;
            while(region_vdd > 0.0 && sr < max_sacrificed_lum_ratio)
            {
                max_gray_level = std::floor(MAXIMUM_GRAY_LEVEL*lookup_gray_level_threshold(region_vdd));
                sr =  sacrificed_luminance_ratio(frame, max_gray_level, total_luminance);
                std::cout << "Max Gray Level: " << max_gray_level << ", S.R.:" << sr<< std::endl;
                region_vdd -= vdd_step; 
            }
            region.vdd(i,j,region_vdd); //take the last that is below the max ratio
        }
    }
}

void adjust_voltage(std::pair<unsigned, unsigned> partition_left, std::pair<unsigned, unsigned> partition_right)
{

}

void temporal_vdd_optimization(Vdd_Regions & region, const Frame & frame, const std::vector<double> & voltage_set)
{

}
